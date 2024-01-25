#include "auction.h"

namespace pokerbot {

Auctioneer::Auctioneer(){};

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

Range* Auctioneer::receive_bid(Range& villain_range, const int villain_bid, const int hero_bid,
                               const int pot, float time_budget) {
  /*TODO: In re exploits I think I like keeping track of the following:
		- keep track of max absolute bid
		- keep track of min absolute bid
		- keep track of max % of pot bid
		- keep track of min % of pot bid
		If there is sufficiently low spread in either value, we make the floor of our minimum bid that value - 1.
		Otherwise we do EV or equity based bidding*/
  if (hero_bid > villain_bid) {
    return &villain_range;
  }
  //TODO: Change villain's range to reflect they received a card after the auction
  return &villain_range;
}

}  // namespace pokerbot
