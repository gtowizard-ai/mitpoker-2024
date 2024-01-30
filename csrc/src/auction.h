#pragma once
#include "game.h"
#include "hand.h"
#include "range.h"

namespace pokerbot {

inline constexpr int ABS_BIDDING_EPSILON = 2;
inline constexpr float POT_PERCENTAGE_BIDDING_EPSILON = .1;
inline constexpr int REASONABLE_DIST_FROM_MAX = 10;
inline constexpr float BID_MULTIPLIER_OOP = 1;
inline constexpr float BID_MULTIPLIER_IP = 1;
inline constexpr int SIGNIFICANT_BID_COUNT = 5;

struct Auctioneer {

  Auctioneer();

  int v_abs_bid_min_max[2];
  float v_pot_percentage_min_max[2];
  bool v_is_excessive_bidder;
  int bid_count = 0;

  int get_bid(const Range& hero_range, const Range& villain_range, const Game& game,
              const std::vector<card_t>& board_cards, const Hand& hand, int pot,
              float time_budget_ms);

<<<<<<< HEAD
  float mean_equity(const Range& range_one, const Range& range_two, const Game& game,
                    const std::vector<card_t>& board);
=======
  float get_avg_equity_third_card(const std::vector<card_t>& board_cards);
>>>>>>> main

  void receive_bid(Range& hero_range, Range& villain_range, int hero_bid, int villain_bid,
                   const Game& game, const std::vector<card_t>& board_cards, int pot,
                   float time_budget_ms);

  void update_exploits(int bid, int pot);
};

}  // namespace pokerbot
