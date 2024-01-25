#include "auction.h"

namespace pokerbot {

Auctioneer::Auctioneer(){};

int Auctioneer::get_bid(const std::vector<Range>& ranges, const std::vector<Card>& board,
                        Hand& hand, float time_budget) {
  std::cout << "I bid" << std::endl;
  return 0;
}
}  // namespace pokerbot
