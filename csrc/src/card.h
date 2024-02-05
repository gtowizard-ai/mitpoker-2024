#pragma once
#include <fmt/core.h>
#include <vector>
#include "definitions.h"

namespace gtowizard {

constexpr unsigned rank_of_card(card_t card) {
  return card / MAX_SUITS;
}

constexpr unsigned suit_of_card(card_t card) {
  return card % MAX_SUITS;
}

constexpr card_t make_card(unsigned rank, unsigned suit) {
  return rank * MAX_SUITS + suit;
}

class Card {
 public:
  /// @brief    Constructs a card object from an integer
  ///           value between 0 and 51.
  ///
  /// @param card number to create card from
  explicit constexpr Card(card_t card) : card_(card) {}

  /// @brief  Creates a Card object from a card string.
  ///
  ///         string format: 'Fs' where
  ///           - F = Facevalue (2,3,4,..,K,A)
  ///           - s = Suit (s,d,c,h)
  /// @param card a string representation of card
  explicit Card(std::string_view card);

  /// @brief   returns the card value itself
  /// @return int between 0 - 51
  constexpr card_t card() const { return card_; }

  constexpr unsigned rank() const { return rank_of_card(card_); }

  constexpr unsigned suit() const { return suit_of_card(card_); }

  /// @brief generates string representation of card.
  /// @return cardstring. Example: As, Kh, ...
  std::string to_string() const { return to_string(card_); }

  /// @brief generates string representation of card.
  /// @return cardstring. Example: As, Kh, ...
  static std::string to_string(card_t card);

  static std::string to_string(const std::vector<card_t>& cards);

  static std::vector<card_t> to_vector(std::string_view card_string);

  auto operator<(const Card& rhs) const -> bool { return card() < rhs.card(); }
  auto operator>(const Card& rhs) const -> bool { return card() > rhs.card(); }

 private:
  card_t card_;
};

}  // namespace gtowizard
