#pragma once
#include <array>
#include "definitions.h"
#include "game.h"

namespace pokerbot {

struct Range {
  std::array<float, NUM_HANDS_POSTFLOP_3CARDS> range;
  NumCards num_cards;

  /// Initialize uniform random range
  Range();

  /// Set probs to zero for hands that are now blocked by board cards
  void update_on_new_board_cards(const Game& game, const std::vector<card_t>& board_cards);

  /// Transform range from 2-card representation to 3-cards
  void to_3_cards_range(const Game& game, const std::vector<card_t>& board_cards);
};

}  // namespace pokerbot
