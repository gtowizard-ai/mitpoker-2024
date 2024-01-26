#include "auction.h"
#include <gtest/gtest.h>
#include <tuple>

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
  std::vector<card_t> board;
  Hand hand("AsAd");
  int pot = 100;
  const auto bid = auctioneer.get_bid(hero_range, villain_range, board, hand, pot, 2.0);
  ASSERT_EQ(bid, 0);
}

TEST_F(AuctionTest, TestReceiveBid) {
  Auctioneer auctioneer;
  Range v_range;
  int villain_bid = 2;
  int hero_bid = 1;
  int pot = 100;
  auto board_cards = Card::to_vector("AcAdAh");
  auctioneer.receive_bid(v_range, villain_bid, hero_bid, game_, board_cards, pot, 2.0);
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
