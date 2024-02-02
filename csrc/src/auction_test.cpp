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
  auto board = Card::to_vector("AcAdAh");
  Hand hand("KsKd");
  int pot = 100;
  const auto bid = auctioneer.get_bid(board, hand, pot);
  ASSERT_EQ(bid, 65);
  pot = 798;
  const auto bid_2 = auctioneer.get_bid(board, hand, pot);
  ASSERT_EQ(bid_2, 1);
  pot = -2;
  const auto bid_3 = auctioneer.get_bid(board, hand, pot);
  ASSERT_EQ(bid_3, 0);
}

TEST_F(AuctionTest, TestGetBidHighDrawingHand) {
  Auctioneer auctioneer;
  auto board = Card::to_vector("6c5c2d");
  Hand hand("Tc8d");
  int pot = 100;
  const auto bid = auctioneer.get_bid(board, hand, pot);
  // Bid higher because our hand really benefits from it
  ASSERT_EQ(bid, 134);
}

TEST_F(AuctionTest, TestReceiveBid) {
  Auctioneer auctioneer;
  Range hero_range;
  Range villain_range;
  int hero_bid = 1;
  int villain_bid = 2;
  int pot = 100;
  auto board = Card::to_vector("AcAdAh");
  auctioneer.receive_bid(hero_range, villain_range, hero_bid, villain_bid, game_, board, pot);
}

TEST_F(AuctionTest, TestUpdateExploits) {
  Auctioneer auctioneer;
  int villain_bid = 391;
  int hero_bid = 390;
  int pot = 10;
  auctioneer.update_exploits(hero_bid, villain_bid, pot + hero_bid);
  ASSERT_TRUE(auctioneer.v_is_excessive_bidder);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[0], 391);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[1], 391);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[0], 39.1, TOLERANCE);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[1], 39.1, TOLERANCE);
  villain_bid = 10;
  hero_bid = 11;
  pot = 10;
  auctioneer.update_exploits(hero_bid, villain_bid, pot + villain_bid);
  ASSERT_FALSE(auctioneer.v_is_excessive_bidder);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[0], 10);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[1], 391);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[0], 1.0, TOLERANCE);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[1], 39.1, TOLERANCE);
  villain_bid = 5;
  hero_bid = 5;
  pot = 10;
  auctioneer.update_exploits(hero_bid, villain_bid, pot + hero_bid + villain_bid);
  ASSERT_FALSE(auctioneer.v_is_excessive_bidder);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[0], 5);
  ASSERT_EQ(auctioneer.v_abs_bid_min_max[1], 391);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[0], .5, TOLERANCE);
  ASSERT_NEAR(auctioneer.v_pot_percentage_min_max[1], 39.1, TOLERANCE);
}
