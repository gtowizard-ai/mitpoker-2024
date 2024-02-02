#pragma once

#include "card.h"
#include "definitions.h"
#include "hand_strength.h"
#include "poker_hand_tables.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace pokerbot {

/// A struct representing a set of cards, designed to efficiently merge and evaluate hands.
class PokerHand;

namespace detail {

/// Basis for each suit.
inline constexpr std::array<uint16_t, MAX_SUITS> SUIT_BASIS = {0x1000, 0x100, 0x10, 0x1};

/// Creates an array of `PokerHand` objects for each card singleton.
constexpr std::array<PokerHand, MAX_DECK_SIZE> init_eval_hand_array() noexcept;

}  // namespace detail

class PokerHand {
 public:
  /// The maximum number of cards in a hand that can be evaluated.
  static constexpr uint8_t MAX_SIZE = 8;

  /// Constructs an empty hand (default constructor).
  constexpr PokerHand() noexcept : misc_data_{0}, suitwise_bitset_{0} {}

  /// @brief Constructs a hand from a set of cards. Throws an exception if `cards` contains
  /// duplicate cards or values outside of the range [0, 51].
  inline PokerHand(std::initializer_list<card_t> cards);

  /// @brief Constructs a hand from a set of cards. Throws an exception if `cards` contains
  /// duplicate cards or values outside of the range [0, 51].
  explicit inline PokerHand(const std::vector<card_t>& cards);

  /// @brief Constructs a hand from a given string. Throws an exception if the string contains
  /// duplicate cards or invalid characters.
  explicit inline PokerHand(std::string_view card_string);

  /// Returns the number of cards in the hand.
  constexpr uint8_t size() const noexcept { return static_cast<uint8_t>(misc_data_ >> 32); }

  /// Returns whether the hand is empty.
  constexpr bool empty() const noexcept { return size() == 0; }

  /// Returns whether the hand contains a given card.
  constexpr bool contains(card_t card) const;

  /// Returns whether the hand contains a given hand.
  constexpr bool contains(const PokerHand& other) const noexcept {
    return (suitwise_bitset_ & other.suitwise_bitset_) == other.suitwise_bitset_;
  }

  /// Returns whether the hand collides with a given hand.
  constexpr bool collides_with(const PokerHand& other) const noexcept {
    return (suitwise_bitset_ & other.suitwise_bitset_) != 0;
  }

  /// @brief Returns the strength of the hand (higher is better). Undefined behavior if the hand
  /// contains more than 8 cards.
  /// @return Higher 13 bits: strength / Lower 3 bits: draw flags.
  /// `STRENGTH_MASK` can be used to extract the strength. Even if the river card has already been
  /// dealt, this function may return draw flags enabled because it does not know about it.
  strength_t evaluate() const {
    constexpr uint64_t flush_adder = 0x3333'0000'0000'0000;
    constexpr uint64_t flush_draw_adder = 0x4444'0000'0000'0000;
    constexpr uint64_t flush_test_mask = 0x8888'0000'0000'0000;

    const auto rank_key = static_cast<detail::rank_key_t>(misc_data_);
    const auto offset = detail::RANK_KEY_OFFSET_TABLE[rank_key >> detail::RANK_KEY_OFFSET_SHIFT];
    auto strength = detail::NONFLUSH_LOOKUP_TABLE[rank_key + offset];

    const auto flush_test = (misc_data_ + flush_adder) & flush_test_mask;

    if (!flush_test) {
      if (strength > WEAKEST_FLUSH) {
        return strength;
      }

      const auto flush_draw_test = (misc_data_ + flush_draw_adder) & flush_test_mask;
      const strength_t flush_draw_flag = flush_draw_test ? FLUSH_DRAW_BIT : 0;
      return strength | flush_draw_flag;
    }

    const auto shift_count = __builtin_clzll(flush_test) * 4;
    const auto flush_suit_bitset = static_cast<uint16_t>(suitwise_bitset_ >> shift_count);
    return std::max(strength, detail::FLUSH_LOOKUP_TABLE[flush_suit_bitset]);
  }

  /// Merges two hands together. Undefined behavior if the two hands contain the same card.
  constexpr PokerHand& operator+=(const PokerHand& other) {
    misc_data_ += other.misc_data_;
    suitwise_bitset_ += other.suitwise_bitset_;
    return *this;
  }

  /// @brief Subtracts a hand from another hand. Undefined behavior if the left hand does not
  /// contain the right hand.
  constexpr PokerHand& operator-=(const PokerHand& other) {
    misc_data_ -= other.misc_data_;
    suitwise_bitset_ -= other.suitwise_bitset_;
    return *this;
  }

  constexpr bool operator==(const PokerHand& other) const noexcept {
    return suitwise_bitset_ == other.suitwise_bitset_;
  }

  constexpr bool operator!=(const PokerHand& other) const noexcept {
    return suitwise_bitset_ != other.suitwise_bitset_;
  }

 private:
  /// An integer that stores miscellaneous data.
  /// @details This data member contains the following information:
  ///   - Bits  0-31 (32 bits): The unique key representing the combination of card ranks.
  ///   - Bits 32-39 ( 8 bits): The number of cards in the hand (see `size()`).
  ///   - Bits 40-47 ( 8 bits): Padding.
  ///   - Bits 48-63 (16 bits): The number of cards in each suit (see `detail::SUIT_BASIS`).
  /// @note Ideally, this should be a union, but I prefer not to use a union here for portability.
  uint64_t misc_data_;

  /// Concatenation of four 16-bit integers, each representing the rank bitset of a suit.
  uint64_t suitwise_bitset_;

  friend constexpr std::array<PokerHand, MAX_DECK_SIZE> detail::init_eval_hand_array() noexcept;
};

/// Merges two hands together. Throws an exception if the two hands contain the same card.
constexpr PokerHand operator+(const PokerHand& lhs, const PokerHand& rhs) {
  PokerHand result(lhs);
  result += rhs;
  return result;
}

/// @brief Subtracts a hand from another hand. Throws an exception if the left hand does not contain
/// the right hand.
constexpr PokerHand operator-(const PokerHand& lhs, const PokerHand& rhs) {
  PokerHand result(lhs);
  result -= rhs;
  return result;
}

namespace detail {

constexpr std::array<PokerHand, MAX_DECK_SIZE> init_eval_hand_array() noexcept {
  std::array<PokerHand, MAX_DECK_SIZE> result;
  for (card_t card = 0; card < MAX_DECK_SIZE; ++card) {
    const auto suit = suit_of_card(card);
    const auto rank = rank_of_card(card);
    result[card].misc_data_ =
        (static_cast<uint64_t>(SUIT_BASIS[suit]) << 48) + (1ULL << 32) + RANK_BASES[rank];
    result[card].suitwise_bitset_ = 1ULL << (suit * 16 + rank);
  }
  return result;
}

/// An array of `PokerHand` objects for each card singleton.
inline constexpr std::array<PokerHand, MAX_DECK_SIZE> EVAL_HAND_ARRAY = init_eval_hand_array();

}  // namespace detail

inline PokerHand::PokerHand(std::initializer_list<card_t> cards) : PokerHand() {
  std::for_each(cards.begin(), cards.end(),
                [&](auto card) { *this += detail::EVAL_HAND_ARRAY[card]; });
}

inline PokerHand::PokerHand(const std::vector<card_t>& cards) : PokerHand() {
  std::for_each(cards.begin(), cards.end(),
                [&](auto card) { *this += detail::EVAL_HAND_ARRAY[card]; });
}

inline PokerHand::PokerHand(std::string_view card_string) : PokerHand() {
  if (card_string.size() % 2 != 0) {
    throw std::invalid_argument("The card string must have an even number of characters.");
  }
  for (unsigned i = 0; i < card_string.size(); i += 2) {
    const auto rank = kRankChars.find(card_string[i]);
    const auto suit = kSuitChars.find(card_string[i + 1]);
    if (rank == std::string_view::npos || suit == std::string_view::npos) {
      throw std::invalid_argument("The card string contains invalid characters.");
    }
    *this += detail::EVAL_HAND_ARRAY[make_card(rank, suit)];
  }
}

constexpr bool PokerHand::contains(card_t card) const {
  return suitwise_bitset_ & detail::EVAL_HAND_ARRAY[card].suitwise_bitset_;
}

}  // namespace pokerbot
