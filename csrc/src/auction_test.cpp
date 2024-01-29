#include "auction.h"
#include <gtest/gtest.h>

using namespace pokerbot;

class AuctionTest : public ::testing::Test {
 protected:
  static constexpr float TOLERANCE = 1e-5;
  Game game_;
};

TEST_F(AuctionTest, TestGetBid) {
  Auctioneer auctioneer;
  Range hero_range;
  Range villain_range;
  auto board = Card::to_vector("AcAdAh");
  Hand hand = Hand("KsKd");
  float time = 2.0;
  int pot = 100;
  const auto bid = auctioneer.get_bid(hero_range, villain_range, game_, board, hand, pot, time);
  ASSERT_EQ(bid, 36);
}

TEST_F(AuctionTest, TestReceiveBid) {
  Auctioneer auctioneer;
  Range hero_range;
  Range villain_range;
  int hero_bid = 1;
  int villain_bid = 2;

  int pot = 100;
  auto board = Card::to_vector("AcAdAh");
  auctioneer.receive_bid(hero_range, villain_range, hero_bid, villain_bid, game_, board, pot, 2.0);
}

TEST_F(AuctionTest, TestUpdateExploits) {
  Auctioneer auctioneer;
  int bid = 390;
  auctioneer.update_exploits(bid, 10);
  ASSERT_TRUE(auctioneer.v_is_excessive_bidder);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[0], 390);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[1], 390);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[0], 39.0, TOLERANCE);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[1], 39.0, TOLERANCE);
  bid = 10;
  auctioneer.update_exploits(bid, 10);
  ASSERT_FALSE(auctioneer.v_is_excessive_bidder);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[0], 10);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[1], 390);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[0], 1.0, TOLERANCE);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[1], 39.0, TOLERANCE);
}

TEST_F(AuctionTest, TestMeanEquityStrategy) {
  Auctioneer auctioneer;
  auto board = Card::to_vector("Tc7d2s");
  float eq = auctioneer.get_avg_equity_third_card(board);
  ASSERT_NEAR(eq, 0.352, 0.01);
}