#include "states.h"

#include <fmt/format.h>
#include <algorithm>
#include <optional>
#include <sstream>

#include "card.h"

namespace gtowizard {

StatePtr RoundState::showdown() const {
  return std::make_shared<TerminalState>(std::array<int, 2>{0, 0}, bids, get_shared());
}

std::vector<Action::Type> RoundState::legal_actions() const {
  if (auction) {
    return std::vector{Action::Type::BID};
  }

  auto active = get_active(button);
  auto continue_cost = bets[1 - active] - bets[active];
  if (continue_cost == 0) {
    // we can only raise the stakes if both players can afford it
    auto bets_forbidden = stacks[0] == 0 || stacks[1] == 0;
    return bets_forbidden ? std::vector{Action::Type::CHECK}
                          : std::vector{Action::Type::CHECK, Action::Type::RAISE};
  }
  // continue_cost > 0
  // similarly, re-raising is only allowed if both players can afford it
  auto raises_forbidden = continue_cost >= stacks[active] || stacks[1 - active] == 0;
  return raises_forbidden
             ? std::vector{Action::Type::FOLD, Action::Type::CALL}
             : std::vector{Action::Type::FOLD, Action::Type::CALL, Action::Type::RAISE};
}

std::array<int, 2> RoundState::raise_bounds() const {
  auto active = get_active(button);
  auto continue_cost = bets[1 - active] - bets[active];
  auto max_contribution = std::min(stacks[active], stacks[1 - active] + continue_cost);
  auto min_contribution =
      std::min(max_contribution, continue_cost + std::max(continue_cost, BIG_BLIND));
  return {bets[active] + min_contribution, bets[active] + max_contribution};
}

StatePtr RoundState::proceed_to_next_round() const {
  if (round() == round::RIVER) {
    return this->showdown();
  }
  bool auction = round() == round::PREFLOP;
  return std::make_shared<RoundState>(1, auction, bids, std::array<int, 2>{0, 0}, stacks, hands,
                                      board_cards, get_shared());
}

StatePtr RoundState::proceed(const Action& action) const {
  auto active = get_active(button);
  switch (action.type) {
    case Action::Type::FOLD: {
      auto delta = active == 0 ? stacks[0] - STARTING_STACK : STARTING_STACK - stacks[1];
      return std::make_shared<TerminalState>(std::array<int, 2>{delta, -1 * delta}, bids,
                                             get_shared());
    }
    case Action::Type::CALL: {
      if (button == 0) {
        // sb calls bb
        return std::make_shared<RoundState>(
            1, auction, bids, std::array<int, 2>{BIG_BLIND, BIG_BLIND},
            std::array<int, 2>{STARTING_STACK - BIG_BLIND, STARTING_STACK - BIG_BLIND}, hands,
            board_cards, get_shared());
      }
      // both players acted
      auto new_bets = bets;
      auto new_stacks = stacks;
      auto contribution = new_bets[1 - active] - new_bets[active];
      new_stacks[active] = new_stacks[active] - contribution;
      new_bets[active] = new_bets[active] + contribution;
      auto state = std::make_shared<RoundState>(button + 1, auction, bids, new_bets, new_stacks,
                                                hands, board_cards, get_shared());
      return state->proceed_to_next_round();
    }
    case Action::Type::CHECK: {
      if ((round() == round::PREFLOP && button > 0) || button > 1) {
        return this->proceed_to_next_round();
      }
      // let opponent act
      return std::make_shared<RoundState>(button + 1, auction, bids, bets, stacks, hands,
                                          board_cards, get_shared());
    }
    case Action::Type::BID: {
      auto new_bids = bids;
      new_bids[active] = -1;
      if (new_bids[1 - active].has_value()) {
        // both players have submitted bids
        auto state = std::make_shared<RoundState>(1, false, new_bids, bets, stacks, hands,
                                                  board_cards, get_shared());
        return state;
      } else {
        return std::make_shared<RoundState>(button + 1, true, new_bids, bets, stacks, hands,
                                            board_cards, get_shared());
      }
    }
    default: {
      // Action::Type::RAISE
      auto new_bets = bets;
      auto new_stacks = stacks;
      auto contribution = action.amount - new_bets[active];
      new_stacks[active] = new_stacks[active] - contribution;
      new_bets[active] = new_bets[active] + contribution;
      return std::make_shared<RoundState>(button + 1, auction, bids, new_bets, new_stacks, hands,
                                          board_cards, get_shared());
    }
  }
}

template <typename Container>
std::string join(const Container& elements, const std::string& separator) {
  std::stringstream ss;

  auto it = elements.begin();
  if (it != elements.end()) {
    ss << *it++;
  }

  for (; it != elements.end(); ++it) {
    ss << separator << *it;
  }

  return ss.str();
}

std::string RoundState::to_string() const {
  std::array<std::string, 2> formatted_hands = {fmt::format(FMT_STRING("{}"), join(hands[0], "")),
                                                fmt::format(FMT_STRING("{}"), join(hands[1], ""))};

  return fmt::format(FMT_STRING("round(button={}, round={}, bets=[{}], stacks=[{}], hands=[{}], "
                                "cards=[{}])"),
                     button, round().to_string(), join(bets, ", "), join(stacks, ", "),
                     join(formatted_hands, ","), Card::to_string(board_cards));
}

std::string TerminalState::to_string() const {
  return fmt::format(FMT_STRING("terminal(deltas=[{}])"), join(deltas, ", "));
}

}  // namespace gtowizard
