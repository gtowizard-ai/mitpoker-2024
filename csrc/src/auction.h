#pragma once
#include <tuple>
#include "card.h"
#include "hand.h"
#include "range.h"

namespace pokerbot {

struct Auctioneer {

  Auctioneer();

  std::tuple<Range, int> get_bid(const std::vector<Range>& ranges, const std::vector<Card>& board,
                                 const Hand& hand, const int pot, float time_budget);

  Range* receive_bid(Range& villain_range, const int villain_bid, const int hero_bid, const int pot,
                     float time_budget);
};

}  // namespace pokerbot
