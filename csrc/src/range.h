#pragma once
#include <array>
#include "definitions.h"
#include "game.h"

namespace pokerbot {

struct Range {
  NumCards num_cards;
  std::array<float, NUM_HANDS_POSTFLOP_3CARDS> range;

  /// Initialize uniform random range
  Range();

  auto num_hands() const { return Game::num_hands(num_cards); }

  /// Set probs to zero for hands that are now blocked by board cards
  void update_on_board_cards(const Game& game, const std::vector<card_t>& board_cards);

  /// Transform range from 2-card representation to 3-cards
  void to_3_cards_range(const Game& game, const std::vector<card_t>& board_cards);
};

}  // namespace pokerbot
