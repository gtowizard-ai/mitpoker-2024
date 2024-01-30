#include "auction.h"
#include <numeric>
#include "definitions.h"
#include "equity.h"
#include "equity_third_card.h"
#include "isomorphic_flop_encoder.h"

namespace pokerbot {

Auctioneer::Auctioneer() : hand_equities_third_card_(HandEquitiesThirdCard()) {
  v_is_excessive_bidder = true;
  v_abs_bid_min_max[0] = STARTING_STACK;
  v_abs_bid_min_max[1] = -1;
  v_pot_percentage_min_max[0] = static_cast<float>(STARTING_STACK);
  v_pot_percentage_min_max[1] = -1;
}

int Auctioneer::get_bid(const Range& hero_range, const Range& villain_range, const Game& game,
                        const std::vector<card_t>& board, const Hand& hand, const int pot,
                        float time_budget_ms) {
  auto isomorphic_board = IsomorphicFlopEncoder::to_isomorphic_flop(board);
  float board_eq_difference = -AVG_EQUITY_LOSS_THIRD_CARD.at(isomorphic_board);
  float hand_eq_difference =
      -hand_equities_third_card_.get_hand_equity_loss_third_card(board, hand);
  fmt::print("For 3rd cards: Hand EQ difference = {} / Board EQ difference = {} \n",
             hand_eq_difference, board_eq_difference);

  float equity_difference = std::max(board_eq_difference, hand_eq_difference);
  float equity_bid = ((1 / (1 - equity_difference)) - 1) * pot;

  int abs_bid_diff = v_abs_bid_min_max[1] - v_abs_bid_min_max[0];
  float rel_bid_diff = v_pot_percentage_min_max[1] - v_pot_percentage_min_max[0];

  //This is the sum of the geometric series of of form
  //x + x^2 + x^3..., which accounts for us getting the equity
  //back from our bid itself
  float equity_with_bid = ((1 / (1 - equity_difference)) - 1) * pot;
  int default_bid = std::ceil(equity_with_bid) + 1;

  if (SIGNIFICANT_BID_COUNT > bid_count) {
    return default_bid;
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
  if (villain_range.num_cards == NumCards::Three || hero_range.num_cards == NumCards::Three) {
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
