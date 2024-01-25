#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include "card.h"
#include "definitions.h"
#include "n_choose_k.h"

namespace pokerbot {

namespace detail {

inline void sort_2_cards(card_t& card1, card_t& card2) {
  if (card1 > card2) {
    std::swap(card1, card2);
  }
}

constexpr uint64_t VALID_CARD_BITSET = (1ULL << MAX_DECK_SIZE) - 1;

// This function does not check if `card < MAX_DECK_SIZE`
template <typename T>
inline uint64_t card_bitset(const T& cards) {
  uint64_t bitset = 0;
  for (auto card : cards) {
    bitset |= 1ULL << card;
  }
  return bitset;
}

}  // namespace detail

/// A hand can hold either 2 or 3 cards.
/// If only 2 cards, the last card will be set to `MAX_DECK_SIZE`
struct Hand {
  std::array<card_t, 3> cards;

  Hand() = default;

  Hand(card_t card1, card_t card2) : cards{card1, card2, MAX_DECK_SIZE} {
    detail::sort_2_cards(cards[0], cards[1]);
  }

  Hand(card_t card1, card_t card2, card_t card3) : cards{card1, card2, card3} {
    detail::sort_2_cards(cards[0], cards[1]);
    detail::sort_2_cards(cards[0], cards[2]);
    detail::sort_2_cards(cards[1], cards[2]);
  }

  explicit Hand(std::string_view card_string) {
    if (card_string.size() != 4 && card_string.size() != 6) {
      throw std::invalid_argument(
          fmt::format("Invalid string provided to create a hand = {}", card_string));
    }
    for (unsigned i = 0; i < card_string.size() / 2; ++i) {
      cards[i] = Card(card_string.substr(i * 2, 2)).card();
    }
    for (unsigned i = card_string.size() / 2; i < cards.size(); ++i) {
      cards[i] = MAX_DECK_SIZE;
    }
    std::sort(cards.begin(), cards.end());
  }

  uint_fast8_t num_cards() const { return cards[2] == MAX_DECK_SIZE ? 2 : 3; }

  card_t high_card() const { return cards[num_cards() - 1]; }

  card_t low_card() const { return cards[0]; }

  // Mapping from {card1, card2} to index over all combos
  // (e.g. from [0, 1326) when two cards, or [0, 22100) with 3 cards
  hand_t index() const {
    if (num_cards() == 2) {
      return cards[0] + n_choose_k(cards[1], 2);
    } else {
      return cards[0] + n_choose_k(cards[1], 2) + n_choose_k(cards[2], 3);
    }
  }

  // Assume that `card < MAX_DECK_SIZE`
  bool collides_with(card_t card) const {
    return std::find(cards.cbegin(), cards.cend(), card) != cards.cend();
  }

  // `board_cards` can have `MAX_DECK_SIZE` to represent the unknown cards
  bool collides_with(const std::vector<card_t>& board_cards) const {
    return (detail::card_bitset(cards) & detail::card_bitset(board_cards) &
            detail::VALID_CARD_BITSET) != 0;
  }

  bool collides_with(const Hand& hand) const {
    return (detail::card_bitset(cards) & detail::card_bitset(hand.cards) &
            detail::VALID_CARD_BITSET) != 0;
  }

  std::string to_string() const {
    std::string str;
    str.reserve(num_cards() * 2);
    for (unsigned i = 0; i < num_cards(); ++i) {
      str += Card(cards[i]).to_string();
    }
    return str;
  }
};

}  // namespace pokerbot
