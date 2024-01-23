#pragma once

#include <iostream>
#include <memory>
#include <unordered_set>
#include <optional>

#include "actions.h"
#include "constants.h"

namespace pokerbots::skeleton {

struct State : public std::enable_shared_from_this<State> {

  virtual ~State() = default;

  friend std::ostream &operator<<(std::ostream &os, const State &s) {
    return s.doFormat(os);
  }

  template <typename Desired = State>
  std::shared_ptr<const Desired> getShared() const {
    return std::static_pointer_cast<const Desired>(shared_from_this());
  }

private:
  virtual std::ostream &doFormat(std::ostream &os) const = 0;
};

using StatePtr = std::shared_ptr<const State>;

struct RoundState : public State {
  int button;
  int street;
  bool auction;
  std::array<std::optional<int>, 2> bids;
  std::array<int, 2> pips;
  std::array<int, 2> stacks;
  std::array<std::array<std::string, 3>, 2> hands;
  std::array<std::string, 5> deck;
  StatePtr previousState;

  RoundState(int button, int street, bool auction, std::array<std::optional<int>, 2> bids, std::array<int, 2> pips,  std::array<int, 2> stacks,
             std::array<std::array<std::string, 3>, 2> hands, std::array<std::string, 5> deck,
             StatePtr previousState)
      : button(button), street(street), auction(auction), bids(std::move(bids)), pips(std::move(pips)), stacks(std::move(stacks)),
        hands(std::move(hands)), deck(std::move(deck)),
        previousState(std::move(previousState)) {}

  StatePtr showdown() const;

  std::unordered_set<Action::Type> legalActions() const;

  std::array<int, 2> raiseBounds() const;

  StatePtr proceedStreet() const;

  StatePtr proceed(Action action) const;

private:
  std::ostream &doFormat(std::ostream &os) const override;
};

using RoundStatePtr = std::shared_ptr<const RoundState>;

struct TerminalState : public State {
  std::array<int, 2> deltas;
  std::array<std::optional<int>, 2> bids;
  StatePtr previousState;

  TerminalState(std::array<int, 2> deltas, std::array<std::optional<int>, 2> bids,StatePtr previousState)
      : deltas(std::move(deltas)), bids(std::move(bids)), previousState(std::move(previousState)) {}

private:
  std::ostream &doFormat(std::ostream &os) const override;
};

using TerminalStatePtr = std::shared_ptr<const TerminalState>;

inline int getActive(int a) {
  auto active = a % 2;
  return active < 0 ? active + 2 : active;
}

} // namespace pokerbots::skeleton
