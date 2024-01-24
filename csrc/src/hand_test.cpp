#include "hand.h"
#include <gtest/gtest.h>
#include <unordered_set>

using namespace pokerbot;

TEST(HandTest, TestSmallest2CardsHand) {
  Hand h_2c2d(0, 1);
  ASSERT_EQ(Hand("2c2d").cards, h_2c2d.cards);
  ASSERT_EQ(Hand("2d2c").index(), h_2c2d.index());
  ASSERT_EQ(h_2c2d.num_cards(), 2);
  ASSERT_EQ(h_2c2d.low_card(), 0);
  ASSERT_EQ(h_2c2d.high_card(), 1);
  ASSERT_EQ(h_2c2d.index(), 0);
  ASSERT_EQ(h_2c2d.to_string(), "2c2d");
}

TEST(HandTest, TestHighest2CardsHand) {
  Hand h_AhAs(50, 51);
  ASSERT_EQ(Hand("AhAs").cards, h_AhAs.cards);
  ASSERT_EQ(Hand("AsAh").index(), h_AhAs.index());
  ASSERT_EQ(h_AhAs.num_cards(), 2);
  ASSERT_EQ(h_AhAs.low_card(), 50);
  ASSERT_EQ(h_AhAs.high_card(), 51);
  ASSERT_EQ(h_AhAs.index(), NUM_HANDS_POSTFLOP_2CARDS - 1);
  ASSERT_EQ(h_AhAs.to_string(), "AhAs");
}

TEST(HandTest, TestSmallest3CardsHand) {
  Hand h_2c2d2h(0, 1, 2);
  ASSERT_EQ(Hand("2h2d2c").cards, h_2c2d2h.cards);
  ASSERT_EQ(Hand("2h2d2c").index(), h_2c2d2h.index());
  ASSERT_EQ(h_2c2d2h.num_cards(), 3);
  ASSERT_EQ(h_2c2d2h.low_card(), 0);
  ASSERT_EQ(h_2c2d2h.high_card(), 2);
  ASSERT_EQ(h_2c2d2h.index(), 0);
  ASSERT_EQ(h_2c2d2h.to_string(), "2c2d2h");
}

TEST(HandTest, TestHighest3CardsHand) {
  Hand h_AdAhAs(49, 50, 51);
  ASSERT_EQ(Hand("AdAhAs").cards, h_AdAhAs.cards);
  ASSERT_EQ(Hand("AdAhAs").index(), h_AdAhAs.index());
  ASSERT_EQ(h_AdAhAs.num_cards(), 3);
  ASSERT_EQ(h_AdAhAs.low_card(), 49);
  ASSERT_EQ(h_AdAhAs.high_card(), 51);
  ASSERT_EQ(h_AdAhAs.index(), NUM_HANDS_POSTFLOP_3CARDS - 1);
  ASSERT_EQ(h_AdAhAs.to_string(), "AdAhAs");
}

TEST(HandTest, Test2CardsIndexing) {
  std::unordered_set<hand_t> indices;
  indices.reserve(NUM_HANDS_POSTFLOP_2CARDS);
  for (card_t card1 = 0; card1 < MAX_DECK_SIZE; ++card1) {
    for (card_t card2 = card1 + 1; card2 < MAX_DECK_SIZE; ++card2) {
      indices.insert(Hand(card1, card2).index());
    }
  }
  ASSERT_EQ(indices.size(), NUM_HANDS_POSTFLOP_2CARDS);
  ASSERT_LT(*std::max_element(indices.begin(), indices.end()), NUM_HANDS_POSTFLOP_2CARDS);
}

TEST(HandTest, Test3CardsIndexing) {
  std::unordered_set<hand_t> indices;
  indices.reserve(NUM_HANDS_POSTFLOP_3CARDS);
  for (card_t card1 = 0; card1 < MAX_DECK_SIZE; ++card1) {
    for (card_t card2 = card1 + 1; card2 < MAX_DECK_SIZE; ++card2) {
      for (card_t card3 = card2 + 1; card3 < MAX_DECK_SIZE; ++card3) {
        indices.insert(Hand(card1, card2, card3).index());
      }
    }
  }
  ASSERT_EQ(indices.size(), NUM_HANDS_POSTFLOP_3CARDS);
  ASSERT_LT(*std::max_element(indices.begin(), indices.end()), NUM_HANDS_POSTFLOP_3CARDS);
}

TEST(HandTest, TestCollidesWith) {
  ASSERT_TRUE(Hand("2c2d").collides_with(Card("2c").card()));
  ASSERT_TRUE(Hand("2c2d").collides_with(Card("2d").card()));
  ASSERT_TRUE(Hand("2c2dAs").collides_with(Card("As").card()));
  ASSERT_FALSE(Hand("2c2dAs").collides_with(Card("Ad").card()));

  ASSERT_TRUE(Hand("2c2d").collides_with(Hand("2c2d2s")));
  ASSERT_TRUE(Hand("2c2d").collides_with(Hand("AsKh2c")));
  ASSERT_TRUE(Hand("2c2dAs").collides_with(Hand("AsAcAd")));
  ASSERT_FALSE(Hand("2c2d").collides_with(Hand("AsKh")));
  ASSERT_FALSE(Hand("AcKd").collides_with(Hand("AsKh")));
  ASSERT_FALSE(Hand("2c2dAs").collides_with(Hand("KcKd")));

  ASSERT_FALSE(Hand("AcKd").collides_with(Card::to_vector("AsKh")));
  ASSERT_TRUE(Hand("AcKd").collides_with(Card::to_vector("AsKd")));
}