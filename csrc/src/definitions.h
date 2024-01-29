#pragma once
#include <array>
#include <cstdint>
#include <optional>
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
constexpr hand_t NUM_HANDS_PREFLOP = 169;
constexpr hand_t NUM_HANDS_POSTFLOP_2CARDS = 1326;
constexpr hand_t NUM_HANDS_POSTFLOP_3CARDS = 22100;

inline constexpr int STARTING_STACK = 400;
inline constexpr int BIG_BLIND = 2;
inline constexpr int SMALL_BLIND = 1;
inline constexpr int SB_POS = 0;
inline constexpr int BB_POS = 1;

inline std::array<int, 2> STARTING_STACKS = {
    STARTING_STACK - SMALL_BLIND,
    STARTING_STACK - BIG_BLIND,
};
inline std::array<int, 2> BLINDS = {SMALL_BLIND, BIG_BLIND};
inline std::array<std::optional<int>, 2> EMPTY_BIDS = {std::nullopt, std::nullopt};

}  // namespace pokerbot
