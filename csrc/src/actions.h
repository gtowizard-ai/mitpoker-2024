#pragma once
#include <string>

namespace pokerbot {

struct Action {
  enum class Type { FOLD, CALL, CHECK, RAISE, BID };

  Type type;
  int amount;

  Action(Type t = Type::CHECK, int a = 0) : type(t), amount(a) {}

  std::string to_string() const;
};

}  // namespace pokerbot
