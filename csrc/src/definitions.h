#pragma once
#include <cstdint>
#include <string_view>

namespace pokerbot {

constexpr std::string_view kSuitChars = "cdhs";
constexpr std::string_view kRankChars = "23456789TJQKA";

// Card in [0, ..., 52) range
using card_t = uint_fast8_t;
// Hand in [0, ..., 1326) or [0, ..., 22100) range
using hand_t = uint_fast32_t;

constexpr int MAX_SUITS = 4;
constexpr int MAX_RANKS = 13;
constexpr int MAX_DECK_SIZE = MAX_SUITS * MAX_RANKS;
constexpr int MAX_BOARD_CARDS = 5;
constexpr int NUM_HANDS_PREFLOP = 169;
constexpr int NUM_HANDS_POSTFLOP_2CARDS = 1326;
constexpr int NUM_HANDS_POSTFLOP_3CARDS = 22100;

inline constexpr int STARTING_STACK = 400;
inline constexpr int BIG_BLIND = 2;
inline constexpr int SMALL_BLIND = 1;

inline constexpr int ABS_BIDDING_EPSILON = 2;
inline constexpr float POT_PERCENTAGE_BIDDING_EPSILON = .1;
inline constexpr int REASONABLE_DIST_FROM_MAX = 10;

}  // namespace pokerbot
