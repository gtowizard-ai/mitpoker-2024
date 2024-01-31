#pragma once
#include <vector>
#include "definitions.h"
#include "game.h"

namespace pokerbot {

inline constexpr unsigned RANGE_SIZE_MULTIPLE = 16;

template <typename T>
inline constexpr T ceil_to_multiple(T n, unsigned multiple = RANGE_SIZE_MULTIPLE) {
  return (n + multiple - 1) & ~(multiple - 1);
}

struct Range {
  // smallest multiple of `RANGE_SIZE_MULTIPLE` that is >= `NUM_HANDS_POSTFLOP_3CARDS`
  static constexpr hand_t SIZE = ceil_to_multiple(NUM_HANDS_POSTFLOP_3CARDS);

  NumCards num_cards;

  // `range.size()` may not be equal to `NUM_HANDS_POSTFLOP_3CARDS`!
  std::vector<float> range;

  /// Initialize uniform random range
  Range();

  auto num_hands() const { return Game::num_hands(num_cards); }

  /// Set probs to zero for hands that are now blocked by board cards
  void update_on_board_cards(const Game& game, const std::vector<card_t>& board_cards);

  /// Transform range from 2-card representation to 3-cards
  void to_3_cards_range(const Game& game, const std::vector<card_t>& board_cards);
};

}  // namespace pokerbot
