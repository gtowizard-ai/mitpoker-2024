#pragma once

#include <iostream>

#include "constants.h"

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace pokerbots::skeleton {

struct Action {
  enum Type { FOLD, CALL, CHECK, RAISE, BID };

  Type actionType;
  int amount;

  Action(Type t = Type::CHECK, int a = 0) : actionType(t), amount(a) {}

  friend std::ostream &operator<<(std::ostream &os, const Action &a);
};

} // namespace pokerbots::skeleton
template <> struct fmt::formatter<pokerbots::skeleton::Action> : ostream_formatter {};
