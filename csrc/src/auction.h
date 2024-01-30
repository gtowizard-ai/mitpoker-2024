#pragma once
#include "equity_third_card.h"
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

class Auctioneer {
 public:
  Auctioneer();

  int v_abs_bid_min_max[2]{};
  float v_pot_percentage_min_max[2]{};
  bool v_is_excessive_bidder;
  int bid_count = 0;

  int get_bid(const Range& hero_range, const Range& villain_range, const Game& game,
              const std::vector<card_t>& board_cards, const Hand& hand, int pot,
              float time_budget_ms);

  void receive_bid(Range& hero_range, Range& villain_range, int hero_bid, int villain_bid,
                   const Game& game, const std::vector<card_t>& board_cards, int pot,
                   float time_budget_ms);

  void update_exploits(int bid, int pot);

 private:
  HandEquitiesThirdCard hand_equities_third_card_;
};

}  // namespace pokerbot
