#include "states.h"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <algorithm>
#include <numeric>
#include <optional>
#include <sstream>

namespace pokerbots::skeleton {

StatePtr RoundState::showdown() const {
  return std::make_shared<TerminalState>(std::array<int, 2>{0, 0}, bids, getShared());
}

std::vector<Action::Type> RoundState::legal_actions() const {
  if (auction) {
    return std::vector{Action::Type::BID};
  }

  auto active = getActive(button);
  auto continue_cost = pips[1 - active] - pips[active];
  if (continue_cost == 0) {
    // we can only raise the stakes if both players can afford it
    auto bets_forbidden = stacks[0] == 0 || stacks[1] == 0;
    return bets_forbidden ? std::vector{Action::Type::CHECK}
                          : std::vector{Action::Type::CHECK, Action::Type::RAISE};
  }
  // continueCost > 0
  // similarly, re-raising is only allowed if both players can afford it
  auto raises_forbidden = continue_cost >= stacks[active] || stacks[1 - active] == 0;
  return raises_forbidden
             ? std::vector{Action::Type::FOLD, Action::Type::CALL}
             : std::vector{Action::Type::FOLD, Action::Type::CALL, Action::Type::RAISE};
}

std::array<int, 2> RoundState::raise_bounds() const {
  auto active = getActive(button);
  auto continue_cost = pips[1 - active] - pips[active];
  auto max_contribution = std::min(stacks[active], stacks[1 - active] + continue_cost);
  auto min_contribution =
      std::min(max_contribution, continue_cost + std::max(continue_cost, BIG_BLIND));
  return {pips[active] + min_contribution, pips[active] + max_contribution};
}

StatePtr RoundState::proceed_street() const {
  if (street == 5) {
    return this->showdown();
  }
  if (street == 0) {
    return std::make_shared<RoundState>(1, 3, true, bids, std::array<int, 2>{0, 0}, stacks, hands,
                                        deck, getShared());
  }
  //auto newStreet = street == 0 ? 3 : street + 1;
  return std::make_shared<RoundState>(1, street + 1, false, bids, std::array<int, 2>{0, 0}, stacks,
                                      hands, deck, getShared());
}

StatePtr RoundState::proceed(Action action) const {
  auto active = getActive(button);
  switch (action.action_type) {
    case Action::Type::FOLD: {
      auto delta = active == 0 ? stacks[0] - STARTING_STACK : STARTING_STACK - stacks[1];
      return std::make_shared<TerminalState>(std::array<int, 2>{delta, -1 * delta}, bids,
                                             getShared());
    }
    case Action::Type::CALL: {
      if (button == 0) {  // sb calls bb
        return std::make_shared<RoundState>(
            1, 0, auction, bids, std::array<int, 2>{BIG_BLIND, BIG_BLIND},
            std::array<int, 2>{STARTING_STACK - BIG_BLIND, STARTING_STACK - BIG_BLIND}, hands, deck,
            getShared());
      }
      // both players acted
      auto new_pips = pips;
      auto new_stacks = stacks;
      auto contribution = new_pips[1 - active] - new_pips[active];
      new_stacks[active] = new_stacks[active] - contribution;
      new_pips[active] = new_pips[active] + contribution;
      auto state = std::make_shared<RoundState>(button + 1, street, auction, bids, new_pips,
                                                new_stacks, hands, deck, getShared());
      return state->proceed_street();
    }
    case Action::Type::CHECK: {
      if ((street == 0 && button > 0) || button > 1) {
        return this->proceed_street();
      }
      // let opponent act
      return std::make_shared<RoundState>(button + 1, street, auction, bids, pips, stacks, hands,
                                          deck, getShared());
    }
    case Action::Type::BID: {
      auto new_bids = bids;
      new_bids[active] = -1;
      if (new_bids[1 - active].has_value()) {  //both players have submitted bids
        auto state = std::make_shared<RoundState>(1, 3, false, new_bids, pips, stacks, hands, deck,
                                                  getShared());
        return state;
      } else {
        return std::make_shared<RoundState>(button + 1, 3, true, new_bids, pips, stacks, hands,
                                            deck, getShared());
      }
    }
    default: {  // Action::Type::RAISE
      auto new_pips = pips;
      auto new_stacks = stacks;
      auto contribution = action.amount - new_pips[active];
      new_stacks[active] = new_stacks[active] - contribution;
      new_pips[active] = new_pips[active] + contribution;
      return std::make_shared<RoundState>(button + 1, street, auction, bids, new_pips, new_stacks,
                                          hands, deck, getShared());
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

std::ostream& RoundState::doFormat(std::ostream& os) const {
  std::array<std::string, 2> formatted_hands = {fmt::format(FMT_STRING("{}"), join(hands[0], "")),
                                                fmt::format(FMT_STRING("{}"), join(hands[1], ""))};

  fmt::print(os,
             FMT_STRING("round(button={}, street={}, pips=[{}], stacks=[{}], hands=[{}], "
                        "deck=[{}])"),
             button, street, join(pips, ", "), join(stacks, ", "), join(formatted_hands, ","),
             join(deck, ", "));
  return os;
}

std::ostream& TerminalState::doFormat(std::ostream& os) const {
  fmt::print(os, FMT_STRING("terminal(deltas=[{}])"), join(deltas, ", "));
  return os;
}

}  // namespace pokerbots::skeleton
