#include "actions.h"

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace pokerbot {

std::ostream& operator<<(std::ostream& os, const Action& a) {
  switch (a.action_type) {
    case Action::Type::FOLD:
      return os << 'F';
    case Action::Type::CALL:
      return os << 'C';
    case Action::Type::CHECK:
      return os << 'K';
    case Action::Type::BID:
      fmt::print(os, FMT_STRING("A{}"), a.amount);
      return os;
    default:
      fmt::print(os, FMT_STRING("R{}"), a.amount);
      return os;
  }
}

}  // namespace pokerbot
