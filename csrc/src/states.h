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

  friend std::ostream& operator<<(std::ostream& os, const State& s) { return s.do_format(os); }

  template <typename Desired = State>
  std::shared_ptr<const Desired> get_shared() const {
    return std::static_pointer_cast<const Desired>(shared_from_this());
  }

 private:
  virtual std::ostream& do_format(std::ostream& os) const = 0;
};

using StatePtr = std::shared_ptr<const State>;

struct RoundState : public State {
  int button;
  round::Round round;
  bool auction;
  std::array<std::optional<int>, 2> bids;
  std::array<int, 2> pips;
  std::array<int, 2> stacks;
  std::array<std::array<std::string, 3>, 2> hands;
  std::array<std::string, 5> deck;
  StatePtr previous_state;

  RoundState(int button, round::Round round, bool auction,
             const std::array<std::optional<int>, 2>& bids, const std::array<int, 2>& pips,
             const std::array<int, 2>& stacks,
             const std::array<std::array<std::string, 3>, 2>& hands,
             const std::array<std::string, 5>& deck, StatePtr previous_state)
      : button(button),
        round(round),
        auction(auction),
        bids(bids),
        pips(pips),
        stacks(stacks),
        hands(hands),
        deck(deck),
        previous_state(std::move(previous_state)) {}

  StatePtr showdown() const;

  std::vector<Action::Type> legal_actions() const;

  auto min_stack() const { return std::min(stacks[0], stacks[1]); }

  std::array<int, 2> raise_bounds() const;

  StatePtr proceed_to_next_round() const;

  StatePtr proceed(Action action) const;

 private:
  std::ostream& do_format(std::ostream& os) const override;
};

using RoundStatePtr = std::shared_ptr<const RoundState>;

struct TerminalState : State {
  std::array<int, 2> deltas;
  std::array<std::optional<int>, 2> bids;
  StatePtr previous_state;

  TerminalState(std::array<int, 2> deltas, std::array<std::optional<int>, 2> bids,
                StatePtr previous_state)
      : deltas(deltas), bids(bids), previous_state(std::move(previous_state)) {}

 private:
  std::ostream& do_format(std::ostream& os) const override;
};

using TerminalStatePtr = std::shared_ptr<const TerminalState>;

inline int get_active(int a) {
  auto active = a % 2;
  return active < 0 ? active + 2 : active;
}

}  // namespace pokerbot
