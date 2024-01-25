#include "auction.h"
#include <gtest/gtest.h>
#include <tuple>

using namespace pokerbot;

class AuctionTest : public ::testing::Test {};

TEST_F(AuctionTest, TestGetBid) {
  Auctioneer auctioneer;
  std::vector<Range> ranges;
  std::vector<Card> board;
  Hand hand = Hand("AsAd");
  float time = 2.0;
  int pot = 100;
  ASSERT_EQ(std::get<1>(auctioneer.get_bid(ranges, board, hand, pot, 2.0)), 0);
}

TEST_F(AuctionTest, TestRecieveBid) {
  Auctioneer auctioneer;
  Range v_range;
  int villain_bid = 2;
  int hero_bid = 1;
  int pot = 100;
  Range* new_range = auctioneer.recieve_bid(v_range, villain_bid, hero_bid, pot, 2.0);
  ASSERT_EQ(new_range, &v_range);
}
