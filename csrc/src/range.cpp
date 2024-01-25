#include "range.h"

namespace pokerbot {

Range::Range() : num_cards(NumCards::Two) {
  std::fill_n(range.begin(), NUM_HANDS_POSTFLOP_2CARDS, 1.0);
  std::fill(range.begin() + NUM_HANDS_POSTFLOP_2CARDS, range.end(), 0);
}

void Range::update_on_new_board_cards(const Game& game, const std::vector<card_t>& board_cards) {
  const auto& hands = game.hands(num_cards);
  for (hand_t i = 0; i < hands.size(); ++i) {
    if (hands[i].collides_with(board_cards)) {
      range[i] = 0;
    }
  }
}

void Range::to_3_cards_range(const Game& game, const std::vector<card_t>& board_cards) {
  num_cards = NumCards::Three;
  // TODO - OPTIMIZE THIS IF NEEDED
  const auto prev_range = std::vector(range.begin(), range.begin() + NUM_HANDS_POSTFLOP_2CARDS);
  std::fill_n(range.begin(), NUM_HANDS_POSTFLOP_2CARDS, 0);

  const auto& two_card_hands = game.hands(NumCards::Two);
  float normalization_factor = MAX_DECK_SIZE - 2;

  for (hand_t i = 0; i < two_card_hands.size(); ++i) {
    for (card_t card = 0; card < MAX_DECK_SIZE; ++card) {
      if (two_card_hands[i].collides_with(board_cards) || two_card_hands[i].collides_with(card)) {
        continue;
      }
      auto idx = Hand(two_card_hands[i].cards[0], two_card_hands[i].cards[1], card).index();
      range[idx] += prev_range[i] / normalization_factor;
    }
  }
}

}  // namespace pokerbot