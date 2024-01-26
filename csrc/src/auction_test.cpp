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
  std::vector<Range> ranges;
  std::vector<Card> board;
  Hand hand = Hand("AsAd");
  float time = 2.0;
  int pot = 100;
  const auto [range, bid] = auctioneer.get_bid(ranges, board, hand, pot, 2.0);
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
  int bid = 190;
  auctioneer.update_exploits(bid, 10);
  ASSERT_FALSE(auctioneer.vIsExcessiveBidder);
  ASSERT_EQ(auctioneer.vAbsBidMinMax[0], 190)
  ASSERT_EQ(auctioneer.vAbsBidMinMax[1], 190)
  ASSERT_NEAR(auctioneer.vPotPercentageMinMax[0], 19.0, TOLERANCE)
  ASSERT_NEAR(auctioneer.vPotPercentageMinMax[1], 19.0, TOLERANCE)
}
