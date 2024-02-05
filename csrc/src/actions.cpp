#include "actions.h"

#include <fmt/format.h>

namespace gtowizard {

std::string Action::to_string() const {
  switch (type) {
    case Type::FOLD:
      return "F";
    case Type::CALL:
      return "C";
    case Type::CHECK:
      return "K";
    case Type::BID:
      return fmt::format("A{}", amount);
    default:
      return fmt::format("R{}", amount);
  }
}

}  // namespace gtowizard
