#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <unordered_set>

#include "actions.h"
#include "constants.h"

namespace pokerbots::skeleton {

struct State : public std::enable_shared_from_this<State> {

  virtual ~State() = default;

  friend std::ostream& operator<<(std::ostream& os, const State& s) { return s.doFormat(os); }

  template <typename Desired = State>
  std::shared_ptr<const Desired> getShared() const {
    return std::static_pointer_cast<const Desired>(shared_from_this());
  }

 private:
  virtual std::ostream& doFormat(std::ostream& os) const = 0;
};

using StatePtr = std::shared_ptr<const State>;

struct RoundState : public State {
  int button;
  int street;  // 0=pf, 3=flop, 4=turn, 5=river
  bool auction;
  std::array<std::optional<int>, 2> bids;
  std::array<int, 2> pips;
  std::array<int, 2> stacks;
  std::array<std::array<std::string, 3>, 2> hands;
  std::array<std::string, 5> deck;
  StatePtr previous_state;

  RoundState(int button, int street, bool auction, const std::array<std::optional<int>, 2>& bids,
             const std::array<int, 2>& pips, const std::array<int, 2>& stacks,
             const std::array<std::array<std::string, 3>, 2>& hands,
             const std::array<std::string, 5>& deck, StatePtr previous_state)
      : button(button),
        street(street),
        auction(auction),
        bids(bids),
        pips(pips),
        stacks(stacks),
        hands(hands),
        deck(deck),
        previous_state(std::move(previous_state)) {}

  StatePtr showdown() const;

  std::unordered_set<Action::Type> legal_actions() const;

  auto min_stack() const { return std::min(stacks[0], stacks[1]); }

  std::array<int, 2> raise_bounds() const;

  StatePtr proceed_street() const;

  StatePtr proceed(Action action) const;

 private:
  std::ostream& doFormat(std::ostream& os) const override;
};

using RoundStatePtr = std::shared_ptr<const RoundState>;

struct TerminalState : public State {
  std::array<int, 2> deltas;
  std::array<std::optional<int>, 2> bids;
  StatePtr previous_state;

  TerminalState(std::array<int, 2> deltas, std::array<std::optional<int>, 2> bids,
                StatePtr previous_state)
      : deltas(deltas), bids(bids), previous_state(std::move(previous_state)) {}

 private:
  std::ostream& doFormat(std::ostream& os) const override;
};

using TerminalStatePtr = std::shared_ptr<const TerminalState>;

inline int getActive(int a) {
  auto active = a % 2;
  return active < 0 ? active + 2 : active;
}

}  // namespace pokerbots::skeleton
