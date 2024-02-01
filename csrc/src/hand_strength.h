#pragma once

#include <cstdint>

namespace pokerbot {

/// The type that represents the strength of a hand. Higher is better.
using strength_t = uint16_t;

inline constexpr strength_t FLUSH_DRAW_BIT = 4;
inline constexpr strength_t STRAIGHT_DRAW_MASK = 3;
inline constexpr strength_t STRENGTH_MASK = ~0 ^ 7;

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

}  // namespace pokerbot
