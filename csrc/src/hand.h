#pragma once
#include <algorithm>
#include <array>
#include "card.h"
#include "definitions.h"
#include "n_choose_k.h"

namespace pokerbot {

/// A hand can hold either 2 or 3 cards.
/// If only 2 cards, the last card will be set to `MAX_DECK_SIZE`
struct Hand {
  std::array<card_t, 3> cards;

  Hand(card_t card1, card_t card2) : Hand(card1, card2, MAX_DECK_SIZE) {}

  Hand(card_t card1, card_t card2, card_t card3) : cards{card1, card2, card3} {
    std::sort(cards.begin(), cards.end());
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

  bool collides_with(card_t card) const {
    return std::any_of(cards.begin(), cards.begin() + num_cards(),
                       [card](auto my_card) { return my_card == card; });
  }

  bool collides_with(const std::vector<card_t>& board_cards) const {
    return std::any_of(board_cards.begin(), board_cards.end(),
                       [&](auto card) { return collides_with(card); });
  }

  bool collides_with(const Hand& hand) const {
    for (uint_fast8_t i = 0; i < std::min(this->num_cards(), hand.num_cards()); ++i) {
      if (cards[i] == hand.cards[i]) {
        return true;
      }
    }
    return false;
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
