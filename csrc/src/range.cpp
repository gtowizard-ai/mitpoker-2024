#include "range.h"

#include "ranges_utils.h"

namespace pokerbot {

namespace {

auto init_random_range() {
  std::array<float, NUM_HANDS_POSTFLOP_3CARDS> range{};
  std::fill_n(range.begin(), NUM_HANDS_POSTFLOP_2CARDS, 1.0);
  return range;
}

}  // namespace

Range::Range() : num_cards(NumCards::Two), range(init_random_range()) {}

void Range::update_on_board_cards(const Game& game, const std::vector<card_t>& board_cards) {
  if (board_cards.empty()) {
    return;
  }

  const auto& hands = game.hands(num_cards);
  for (hand_t i = 0; i < hands.size(); ++i) {
    if (hands[i].collides_with(board_cards)) {
      range[i] = 0;
    }
  }
}

// TODO -> We'd ideally need to take into account opponent's range here
void Range::to_3_cards_range(const Game& game, const std::vector<card_t>& board_cards) {
  if (num_cards != NumCards::Two) {
    throw std::runtime_error("Range is already defined over three cards");
  }
  num_cards = NumCards::Three;
  // TODO - OPTIMIZE THIS IF NEEDED
  const auto prev_range = std::vector(range.begin(), range.begin() + NUM_HANDS_POSTFLOP_2CARDS);
  std::fill_n(range.begin(), NUM_HANDS_POSTFLOP_2CARDS, 0);

  const auto& two_card_hands = game.hands(NumCards::Two);
  const float normalization_factor = MAX_DECK_SIZE - board_cards.size() - 2;

  for (card_t card = 0; card < MAX_DECK_SIZE; ++card) {
    if (ranges::contains(board_cards, card)) {
      continue;
    }
    auto full_cards = board_cards;
    full_cards.push_back(card);

    for (hand_t i = 0; i < two_card_hands.size(); ++i) {
      if (two_card_hands[i].collides_with(full_cards)) {
        continue;
      }
      auto idx = Hand(two_card_hands[i].cards[0], two_card_hands[i].cards[1], card).index();
      range[idx] += prev_range[i] / normalization_factor;
    }
  }
}

}  // namespace pokerbot
