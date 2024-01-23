#pragma once

#include <algorithm>

namespace pokerbots::skeleton {

template <typename Container> bool isEmpty(Container &&c) {
  return std::all_of(c.begin(), c.end(), [](auto &&v) { return v.empty(); });
}

} // namespace pokerbots::skeleton
