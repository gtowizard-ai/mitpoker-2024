#include "auction.h"
#include <iostream>

namespace pokerbot {

Auctioneer::Auctioneer(){};

int Auctioneer::get_bid(const std::vector<Range>& ranges, const std::vector<Card>& board,
                        const Hand& hand, float time_budget) {
  return 0;
}
}  // namespace pokerbot
