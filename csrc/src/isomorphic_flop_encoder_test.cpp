#include "isomorphic_flop_encoder.h"
#include <gtest/gtest.h>
#include <unordered_set>

using namespace pokerbot;

TEST(IsomorphicFlopEncoderTest, TestEncodeAllFlops) {
  std::unordered_set<std::string> seen_flops;
  for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
    for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
      for (card_t k = j + 1; k < MAX_DECK_SIZE; ++k) {
        std::vector cards = {i, j, k};
        auto board = IsomorphicFlopEncoder::to_isomorphic_flop(cards);
        seen_flops.insert(board);
      }
    }
  }
  ASSERT_EQ(seen_flops.size(), IsomorphicFlopEncoder::NUM_FLOPS);
}
