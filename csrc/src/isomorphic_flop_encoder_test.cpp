#include "isomorphic_flop_encoder.h"
#include <gtest/gtest.h>
#include <unordered_set>

using namespace gtowizard;

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

TEST(IsomorphicFlopEncoderTest, TestFindMapping) {
  std::vector<int> mapping{3, 0, 1, 2};  // c->s, d->c, h->d, s->h
  ASSERT_EQ(
      IsomorphicFlopEncoder::find_mapping(Card::to_vector("AcKdTh"), Card::to_vector("AsKcTd")),
      mapping);
}
