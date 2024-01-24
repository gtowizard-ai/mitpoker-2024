#pragma once
#include <string_view>

namespace pokerbot {

constexpr std::string_view kSuitChars = "cdhs";
constexpr std::string_view kRankChars = "23456789TJQKA";

// Card in [0, ..., 51] range
using card_t = uint_fast8_t;

constexpr int MAX_SUITS = 4;
constexpr int MAX_RANKS = 13;
constexpr int MAX_DECK_SIZE = MAX_SUITS * MAX_RANKS;
constexpr int MAX_BOARD_CARDS = 5;
constexpr int NUM_HANDS_PREFLOP = 169;
constexpr int NUM_HANDS_POSTFLOP = 1326;

inline constexpr int STARTING_STACK = 400;
inline constexpr int BIG_BLIND = 2;
inline constexpr int SMALL_BLIND = 1;

}  // namespace pokerbot
