#include "equity_third_card.h"
#include <gtest/gtest.h>
#include "isomorphic_flop_encoder.h"
#include "ranges_utils.h"

using namespace pokerbot;

TEST(EquityThirdCardTest, TestAvgEquityThirdCardTableIntegrity) {
  ASSERT_EQ(AVG_EQUITY_LOSS_THIRD_CARD.size(), 1755);
  ASSERT_EQ(ISOMORPHIC_FLOPS.size(), 1755);
  for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
    for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
      for (card_t k = j + 1; k < MAX_DECK_SIZE; ++k) {
        std::vector cards = {i, j, k};
        auto board = IsomorphicFlopEncoder::to_isomorphic_flop(cards);
        ASSERT_TRUE(ranges::contains(ISOMORPHIC_FLOPS, board));
        ASSERT_GE(AVG_EQUITY_LOSS_THIRD_CARD.at(board), -1.0);
        ASSERT_LT(AVG_EQUITY_LOSS_THIRD_CARD.at(board), 0);
      }
    }
  }
}

TEST(EquityThirdCardTest, TestHandEquitiesThirdCard) {
  HandEquitiesThirdCard hand_eqs;

  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("7c8d")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("Ac2d")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("Ac2c")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("Ad2c")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("Ah2s")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("Ah2h")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("2c2d")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("2c2h")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("2c2s")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("2d2h")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("2d2s")));
  fmt::print("{} \n", hand_eqs.get_hand_equity(Card::to_vector("2h2s3c"), Hand("2h2s")));

  auto eq = hand_eqs.get_hand_equity(Card::to_vector("6c5c3h"), Hand("Ac2d"));
  ASSERT_EQ(eq, 1.0);
}