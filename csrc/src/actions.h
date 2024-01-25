#pragma once
#include <string>

namespace pokerbot {

struct Action {
  enum class Type { FOLD, CALL, CHECK, RAISE, BID };

  Type action_type;
  int amount;

  Action(Type t = Type::CHECK, int a = 0) : action_type(t), amount(a) {}

  std::string to_string() const;
};

}  // namespace pokerbot
