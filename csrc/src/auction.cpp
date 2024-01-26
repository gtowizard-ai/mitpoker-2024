#include "auction.h"
#include "definitions.h"

namespace pokerbot {

Auctioneer::Auctioneer() {
  bool vIsExcessiveBidder = true;
  int vAbsBidMinMax[2] = {STARTING_STACK, -1};
  int vPotPercentageMinMax[2] = {STARTING_STACK, -1};
};

std::tuple<Range, int> Auctioneer::get_bid(const std::vector<Range>& ranges,
                                           const std::vector<Card>& board, const Hand& hand,
                                           const int pot, float time_budget) {
  Range range;
  /*TODO: Implement a "slow, fast, very_fast" bidding method as follows:
  	If we are making good time, do a CFR calculation and bid 1/(1-EVdiff) - 1
  	If we are not making good time, do an equity calculation and bid 1/(1-EQdiff) - 1
  	If we are making extremely bad time, bid a constant value (3/4ths of the pot, perhaps)
  	
  	We may want to pass this function what hand we're on (e.g. being on hand 750 with 10s to go isn't a big deal but being on hand 100 with 10s is)
  */
  //TODO: Implement exploitative bidding based on previous received values
  return std::make_tuple(range, 0);
}

void Auctioneer::update_exploits(const int bid, const int pot) {
  //TODO: Write tests for this
  int stack = STARTING_STACK - pot / 2;
  if ((stack - bid) > REASONABLE_DIST_FROM_MAX) {
    this->vIsExcessiveBidder = false;
  }
  if (bid < vAbsBidMinMax[0]) {
    vAbsBidMinMax[0] = bid;
  }
  if (bid > vAbsBidMinMax[1]) {
    vAbsBidMinMax[1] = bid;
  }
  if (static_cast<float>(bid) / pot < vPotPercentageMinMax[0]) {
    vPotPercentageMinMax[0] = bid;
  }
  if (static_cast<float>(bid) / pot > vPotPercentageMinMax[1]) {
    vPotPercentageMinMax[1] = bid;
  }
}

void Auctioneer::receive_bid(Range& villain_range, const int villain_bid, const int hero_bid,
                             const Game& game, const std::vector<card_t>& board, const int pot,
                             float time_budget) {
  update_exploits(villain_bid, pot);
  if (hero_bid > villain_bid) {
    return;
  }

  villain_range.to_3_cards_range(game, board);
  return;
}

}  // namespace pokerbot
