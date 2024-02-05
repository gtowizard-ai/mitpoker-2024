#include "card.h"

namespace gtowizard {

Card::Card(std::string_view card) {
  if (card.size() != 2) {
    throw std::invalid_argument(fmt::format("Invalid Card provided = {}", card));
  }
  auto rank = kRankChars.find(card[0]);
  auto suit = kSuitChars.find(card[1]);
  if (rank >= kRankChars.size() || suit >= kSuitChars.size()) {
    throw std::invalid_argument(fmt::format("Invalid Card provided = {}", card));
  }
  card_ = make_card(rank, suit);
}

std::string Card::to_string(card_t card) {
  auto rank = rank_of_card(card);
  auto suit = suit_of_card(card);
  std::string card_str;
  card_str += kRankChars[rank];
  card_str += kSuitChars[suit];
  return card_str;
}

std::string Card::to_string(const std::vector<card_t>& cards) {
  std::string str;
  str.reserve(2 * cards.size());
  for (auto card : cards) {
    str += to_string(card);
  }
  return str;
}

std::vector<card_t> Card::to_vector(std::string_view card_string) {
  if (card_string.size() % 2 != 0) {
    throw std::invalid_argument(fmt::format("Invalid card_string provided = {}", card_string));
  }

  std::vector<card_t> cards;
  cards.reserve(card_string.size() / 2);

  for (unsigned i = 0; i < card_string.size(); i += 2) {
    cards.push_back(Card(card_string.substr(i, 2)).card());
  }

  return cards;
}

}  // namespace gtowizard
