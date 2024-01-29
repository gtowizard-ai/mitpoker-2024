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

int Auctioneer::get_bid(const Range& hero_range, const Range& villain_range, const Game& game,
                        const std::vector<card_t>& board, const Hand& hand, const int pot,
                        float time_budget_ms) {
  //TODO: Implement exploitative bidding based on previous received values
  Range heroThreeCard = hero_range;
  heroThreeCard.to_3_cards_range(game, board);

  Range villainThreeCard = villain_range;
  villainThreeCard.to_3_cards_range(game, board);
  //TODO: Implement time efficient way for calculating equity
  //TODO: Add cohesive get_bid test to testing
  /*float heroThreeEq = mean_equity(heroThreeCard, villain_range, game, board);
  float heroTwoCard = mean_equity(hero_range, villainThreeCard, game, board);
  float equityDifference = heroThreeEq - heroTwoCard;*/
  float equityDifference = .2;

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
  float bid_to_pot = static_cast<float>(bid) / static_cast<float>(pot);

  if (bid_to_pot < v_pot_percentage_min_max[0]) {
    v_pot_percentage_min_max[0] = bid_to_pot;
  }
  if (bid_to_pot > v_pot_percentage_min_max[1]) {
    v_pot_percentage_min_max[1] = bid_to_pot;
  }
}

void Auctioneer::receive_bid(Range& hero_range, Range& villain_range, const int hero_bid,
                             const int villain_bid, const Game& game,
                             const std::vector<card_t>& board_cards, const int pot,
                             float time_budget_ms) {
  if (villain_range.num_cards == NumCards::Three) {
    return;  // We've already been here on the same hand
  }

  update_exploits(villain_bid, pot);

  // Update ranges based on who won the bid
  if (hero_bid == villain_bid) {
    hero_range.to_3_cards_range(game, board_cards);
    villain_range.to_3_cards_range(game, board_cards);
  } else if (hero_bid > villain_bid) {
    hero_range.to_3_cards_range(game, board_cards);
  } else {
    villain_range.to_3_cards_range(game, board_cards);
  }
}

}  // namespace pokerbot
