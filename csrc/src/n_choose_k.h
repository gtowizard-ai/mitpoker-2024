#pragma once
#include <fmt/format.h>
#include "definitions.h"

namespace pokerbot {

namespace detail {

// Precompute an array of N choose K (for a given max_choose_k)
// where the entry [n * max_choose_k + k] is "N choose K"
template <std::size_t max_choose_k>
constexpr std::array<uint64_t, max_choose_k * max_choose_k> init_choose_array() {
  std::array<uint64_t, max_choose_k* max_choose_k> choose_array = {};
  for (std::size_t i = 0; i < max_choose_k; ++i) {
    // N choose 0 = 1 and N choose N = 1
    choose_array[i * max_choose_k] = 1;
    choose_array[i * max_choose_k + i] = 1;
  }
  // Use Pascal's rule to compute all binomial coefficients
  for (std::size_t i = 1; i < max_choose_k; ++i) {
    for (std::size_t j = 1; j < i; ++j) {
      choose_array[i * max_choose_k + j] =
          choose_array[(i - 1) * max_choose_k + (j - 1)] + choose_array[(i - 1) * max_choose_k + j];
    }
  }
  return choose_array;
}

// It is better to make this variable `static constexpr` in `n_choose_k`,
// but it requires P2647R1 to be implemented (GCC 13 / Clang 16 or later).
inline constexpr auto n_choose_k_array = init_choose_array<MAX_DECK_SIZE + 1>();
}  // namespace detail

// Will only work if N <= `MAX_DECK_SIZE`
constexpr uint64_t n_choose_k(int n, int k) {
  if (n > MAX_DECK_SIZE) {
    throw std::invalid_argument(
        fmt::format("Only accept N <= {} for n_choose_k: Received {}", MAX_DECK_SIZE, n));
  }
  if (k > MAX_DECK_SIZE) {
    throw std::invalid_argument(
        fmt::format("Only accept K <= {} for n_choose_k: Received {}", MAX_DECK_SIZE, k));
  }
  return detail::n_choose_k_array[n * (MAX_DECK_SIZE + 1) + k];
}

}  // namespace pokerbot
