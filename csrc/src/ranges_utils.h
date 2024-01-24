#pragma once

namespace ranges {

template <class R, class T>
bool contains(const R& r, const T& value) {
  return std::find(r.begin(), r.end(), value) != r.end();
}

}  // namespace ranges
