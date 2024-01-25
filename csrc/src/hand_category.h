#pragma once

#include <cstdint>

namespace pokerbot {

/// The type that represents the strength of a hand. Higher is better.
using strength_t = uint16_t;

/// The rank category of a hand. Higher is better.
enum class HandCategory : uint8_t {
  HighCard,
  Pair,
  TwoPair,
  ThreeOfAKind,
  Straight,
  Flush,
  FullHouse,
  FourOfAKind,
  StraightFlush,
};

namespace detail {

/// The number of bits to store the strength within the same hand category.
inline constexpr unsigned HAND_CATEGORY_SHIFT = 12;

}  // namespace detail

/// Gets the rank category of a hand from a given strength value.
constexpr HandCategory hand_category(strength_t strength) noexcept {
  return static_cast<HandCategory>(strength >> detail::HAND_CATEGORY_SHIFT);
}

}  // namespace pokerbot
