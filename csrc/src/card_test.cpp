#include "card.h"
#include <gtest/gtest.h>

using namespace pokerbot;

TEST(CardTest, TestConstructor) {
  ASSERT_EQ(Card(0).card(), 0);
  ASSERT_EQ(Card(0).rank(), 0);
  ASSERT_EQ(Card(0).suit(), 0);
  ASSERT_EQ(Card(0).to_string(), "2c");
  ASSERT_EQ(Card("2c").card(), 0);

  ASSERT_EQ(Card(51).card(), 51);
  ASSERT_EQ(Card(51).rank(), 12);
  ASSERT_EQ(Card(51).suit(), 3);
  ASSERT_EQ(Card(51).to_string(), "As");
  ASSERT_EQ(Card("As").card(), 51);

  ASSERT_ANY_THROW(Card("Ass"));
  ASSERT_ANY_THROW(Card("sA"));
}

TEST(CardTest, TestToVector) {
  ASSERT_EQ(Card::to_vector("Ac").size(), 1);
  ASSERT_EQ(Card::to_vector("As"), std::vector<card_t>{51});
  ASSERT_EQ(Card::to_vector("AcKd2h").size(), 3);
  ASSERT_EQ(Card::to_vector("AcKd2h2sKd").size(), 5);
}