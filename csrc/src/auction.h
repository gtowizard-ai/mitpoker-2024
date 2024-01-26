#pragma once
#include <tuple>
#include "card.h"
#include "game.h"
#include "hand.h"
#include "range.h"

namespace pokerbot {

struct Auctioneer {

  Auctioneer();

  int vAbsBidMinMax[2];
  float vPotPercentageMinMax[2];
  bool vIsExcessiveBidder;

  std::tuple<Range, int> get_bid(const std::vector<Range>& ranges, const std::vector<Card>& board,
                                 const Hand& hand, const int pot, float time_budget);

  void receive_bid(Range& villain_range, const int villain_bid, const int hero_bid,
                   const Game& game, const std::vector<card_t>& board_cards, const int pot,
                   float time_budget);

  void update_exploits(const int bid, const int pot);
};

}  // namespace pokerbot
