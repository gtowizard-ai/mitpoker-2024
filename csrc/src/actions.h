#pragma once
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace pokerbot {

struct Action {
  enum Type { FOLD, CALL, CHECK, RAISE, BID };

  Type action_type;
  int amount;

  Action(Type t = CHECK, int a = 0) : action_type(t), amount(a) {}

  friend std::ostream& operator<<(std::ostream& os, const Action& a);
};

}  // namespace pokerbot
template <>
struct fmt::formatter<pokerbot::Action> : ostream_formatter {};
