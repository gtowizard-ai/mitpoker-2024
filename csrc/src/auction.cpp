#include "auction.h"
#include <cmath>
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
  Range hero_three_card = hero_range;
  hero_three_card.to_3_cards_range(game, board);

  Range villain_three_card = villain_range;
  villain_three_card.to_3_cards_range(game, board);
  //TODO: Implement time efficient way for calculating equity
  //TODO: Add cohesive get_bid test to testing
  /*float heroThreeEq = mean_equity(hero_three_card, villain_range, game, board);
  float heroTwoCard = mean_equity(hero_range, villain_three_card, game, board);
  float equity_difference = heroThreeEq - heroTwoCard;*/
  float equity_difference = .2;
  int abs_bid_diff = v_abs_bid_min_max[1] - v_abs_bid_min_max[0];
  float rel_bid_diff = v_pot_percentage_min_max[1] - v_pot_percentage_min_max[0];

  //This is the sum of the geometric series of of form
  //x + x^2 + x^3..., which accounts for us getting the equity
  //back from our bid itself
  float equity_with_bid = ((1 / (1 - equity_difference)) - 1) * pot;
  int default_bid = std::ceil(equity_with_bid) + 1;

  if (SIGNIFICANT_BID_COUNT > bid_count) {
    return default_bid + 1;
  }
  if (v_is_excessive_bidder) {
    int stack = STARTING_STACK - (pot / 2);
    return stack - REASONABLE_DIST_FROM_MAX;
  }
  if (std::abs(abs_bid_diff) < ABS_BIDDING_EPSILON) {
    std::vector<int> bids = {default_bid, v_abs_bid_min_max[0]};
    return *std::max_element(bids.begin(), bids.end());
  }
  if (std::abs(abs_bid_diff) < POT_PERCENTAGE_BIDDING_EPSILON) {
    //We floor here because we are trying to undercut villain
    std::vector<int> bids = {default_bid,
                             static_cast<int>(std::floor(v_pot_percentage_min_max[0] * pot))};
    return *std::max_element(bids.begin(), bids.end());
  }
  return default_bid;
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
  bid_count++;
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
