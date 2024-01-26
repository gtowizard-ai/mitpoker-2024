#include "auction.h"
#include <numeric>
#include "definitions.h"
#include "equity.h"

namespace pokerbot {

Auctioneer::Auctioneer() {
  v_is_excessive_bidder = true;
  v_abs_bid_min_max[0] = STARTING_STACK;
  v_abs_bid_min_max[1] = -1;
  v_pot_percentage_min_max[0] = static_cast<float>(STARTING_STACK);
  v_pot_percentage_min_max[1] = -1;
};

float Auctioneer::mean_equity(const Range& range_one, const Range& range_two, const Game& game,
                              const std::vector<card_t>& board) {

  std::vector<float> eq = compute_equities(game, range_one, range_two, board);
  float sum = std::accumulate(eq.begin(), eq.end(), 0.0);
  return eq.empty() ? 0.0 : sum / eq.size();
}

int Auctioneer::get_bid(const std::vector<Range>& ranges, const Game& game,
                        const std::vector<card_t>& board, const Hand& hand, const int pot,
                        float time_budget) {
  /*TODO: Implement a "slow, fast, very_fast" bidding method as follows:
  	If we are making good time, do a CFR calculation and bid 1/(1-EVdiff) - 1
  	If we are not making good time, do an equity calculation and bid 1/(1-EQdiff) - 1
  	If we are making extremely bad time, bid a constant value (3/4ths of the pot, perhaps)
  	
  	We may want to pass this function what hand we're on (e.g. being on hand 750 with 10s to go isn't a big deal but being on hand 100 with 10s is)
  */
  //TODO: Implement exploitative bidding based on previous received values
  Range heroThreeCard = ranges[0];
  heroThreeCard.to_3_cards_range(game, board);

  Range villainThreeCard = ranges[1];
  villainThreeCard.to_3_cards_range(game, board);

  float heroThreeEq = mean_equity(heroThreeCard, ranges[1], game, board);
  float heroTwoCard = mean_equity(ranges[0], villainThreeCard, game, board);
  float equityDifference = heroThreeEq - heroTwoCard;

  float equityBid = ((1 / (1 - equityDifference)) - 1) * pot;

  return equityBid;
}

void Auctioneer::update_exploits(const int bid, const int pot) {
  int stack = STARTING_STACK - (pot / 2);
  if ((stack - bid) > REASONABLE_DIST_FROM_MAX) {
    v_is_excessive_bidder = false;
  }
  if (bid < v_abs_bid_min_max[0]) {
    v_abs_bid_min_max[0] = bid;
  }
  if (bid > v_abs_bid_min_max[1]) {
    v_abs_bid_min_max[1] = bid;
  }
  float bidToPot = static_cast<float>(bid) / static_cast<float>(pot);

  if (bidToPot < v_pot_percentage_min_max[0]) {
    v_pot_percentage_min_max[0] = bidToPot;
  }
  if (bidToPot > v_pot_percentage_min_max[1]) {
    v_pot_percentage_min_max[1] = bidToPot;
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
