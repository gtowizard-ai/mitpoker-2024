#include "game.h"
#include <gtest/gtest.h>

using namespace gtowizard;

class GameTest : public ::testing::Test {
 protected:
  Game game_;
};

TEST_F(GameTest, TestTwoCardsHands) {
  const auto& two_cards_hands = game_.hands(NumCards::Two);
  ASSERT_EQ(two_cards_hands.size(), NUM_HANDS_POSTFLOP_2CARDS);
  for (unsigned i = 0; i < two_cards_hands.size(); ++i) {
    ASSERT_EQ(two_cards_hands[i].index(), i);
    ASSERT_EQ(two_cards_hands[i].num_cards(), 2);
  }
}

TEST_F(GameTest, TestThreeCardsHands) {
  const auto& three_cards_hands = game_.hands(NumCards::Three);
  ASSERT_EQ(three_cards_hands.size(), NUM_HANDS_POSTFLOP_3CARDS);
  for (unsigned i = 0; i < three_cards_hands.size(); ++i) {
    ASSERT_EQ(three_cards_hands[i].index(), i);
    ASSERT_EQ(three_cards_hands[i].num_cards(), 3);
  }
}