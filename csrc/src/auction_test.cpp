#include "auction.h"
#include <gtest/gtest.h>


using namespace pokerbot;

class AuctionTest : public ::testing::Test {
};


TEST_F(AuctionTest, TestGetBid) {
  Auctioneer auctioneer;
  std::vector<Range> ranges;
  std::vector<Card> board;
  Hand hand = Hand("AsAd");
  ASSERT_EQ(auctioneer.get_bid(ranges, board, hand, 2.0), 0);
}

