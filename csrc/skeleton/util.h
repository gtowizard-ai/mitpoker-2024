#pragma once

#include <algorithm>

namespace pokerbots::skeleton {

template <typename Container>
bool is_empty(Container&& c) {
  return std::all_of(c.begin(), c.end(), [](auto&& v) { return v.empty(); });
}

}  // namespace pokerbots::skeleton
