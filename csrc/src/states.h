#pragma once
#include <memory>
#include <optional>
#include <vector>
#include "actions.h"
#include "definitions.h"
#include "round.h"

namespace pokerbot {

struct State : std::enable_shared_from_this<State> {
  virtual ~State() = default;

  template <typename Desired = State>
  std::shared_ptr<const Desired> get_shared() const {
    return std::static_pointer_cast<const Desired>(shared_from_this());
  }

  virtual std::string to_string() const = 0;
};

using StatePtr = std::shared_ptr<const State>;

struct RoundState final : State {
  int button;
  bool auction;
  std::array<std::optional<int>, 2> bids;
  std::array<int, 2> bets;
  std::array<int, 2> stacks;
  std::array<std::string, 2> hands;
  std::vector<card_t> board_cards;
  StatePtr previous_state;

  RoundState(int button, bool auction, const std::array<std::optional<int>, 2>& bids,
             const std::array<int, 2>& bets, const std::array<int, 2>& stacks,
             const std::array<std::string, 2>& hands, const std::vector<card_t>& board_cards = {},
             StatePtr previous_state = nullptr)
      : button(button),
        auction(auction),
        bids(bids),
        bets(bets),
        stacks(stacks),
        hands(hands),
        board_cards(board_cards),
        previous_state(std::move(previous_state)) {}

  StatePtr showdown() const;

  std::vector<Action::Type> legal_actions() const;

  auto effective_stack() const { return std::min(stacks[0], stacks[1]); }

  auto effective_stack_start_round() const {
    return std::min(stacks[0] + bets[0], stacks[1] + bets[1]);
  }

  auto pot() const { return 2 * STARTING_STACK - stacks[0] - stacks[1]; }

  // Chips in the middle at start of the round
  auto pot_start_round() const { return pot() - bets[0] - bets[1]; }

  // Amount invested so far during the hand - across all rounds
  auto spent_total(int player) const { return STARTING_STACK - stacks[player]; }

  // the smallest and largest numbers of chips for a legal bet/raise
  std::array<int, 2> raise_bounds() const;

  StatePtr proceed_to_next_round() const;

  StatePtr proceed(const Action& action) const;

  std::string to_string() const override;

  const auto& round() const { return round::from_num_cards(board_cards.size()); }
};

using RoundStatePtr = std::shared_ptr<const RoundState>;

struct TerminalState final : State {
  std::array<int, 2> deltas;
  std::array<std::optional<int>, 2> bids;
  StatePtr previous_state;

  TerminalState(std::array<int, 2> deltas, std::array<std::optional<int>, 2> bids,
                StatePtr previous_state)
      : deltas(deltas), bids(bids), previous_state(std::move(previous_state)) {}

  std::string to_string() const override;
};

using TerminalStatePtr = std::shared_ptr<const TerminalState>;

inline int get_active(int a) {
  auto active = a % 2;
  return active < 0 ? active + 2 : active;
}

}  // namespace pokerbot
