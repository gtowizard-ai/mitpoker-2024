#pragma once
#include <tuple>
#include "card.h"
#include "game.h"
#include "hand.h"
#include "range.h"

namespace pokerbot {

inline constexpr int ABS_BIDDING_EPSILON = 2;
inline constexpr float POT_PERCENTAGE_BIDDING_EPSILON = .1;
inline constexpr int REASONABLE_DIST_FROM_MAX = 10;

struct Auctioneer {

  Auctioneer();

  int v_abs_bid_min_max[2];
  float v_pot_percentage_min_max[2];
  bool v_is_excessive_bidder;

  int get_bid(const std::vector<Range>& ranges, const Game& game, const std::vector<card_t>& board,
              const Hand& hand, const int pot, float time_budget);

  float mean_equity(const Range& range_one, const Range& range_two, const Game& game,
                    const std::vector<card_t>& board);

  void receive_bid(Range& villain_range, const int villain_bid, const int hero_bid,
                   const Game& game, const std::vector<card_t>& board_cards, const int pot,
                   float time_budget);

  void update_exploits(const int bid, const int pot);
};

}  // namespace pokerbot
