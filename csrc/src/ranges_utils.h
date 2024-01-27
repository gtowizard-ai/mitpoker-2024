#pragma once
#include <algorithm>
#include <numeric>

namespace ranges {

template <class R, class T>
bool contains(const R& r, const T& value) {
  return std::find(r.begin(), r.end(), value) != r.end();
}

template <class R>
double sum(const R& r) {
  return std::accumulate(r.begin(), r.end(), 0.0);
}

template <class R>
size_t argmax(const R& r) {
  return std::distance(r.begin(), std::max_element(r.begin(), r.end()));
}

template <class R>
size_t argmin(const R& r) {
  return std::distance(r.begin(), std::min_element(r.begin(), r.end()));
}

}  // namespace ranges
