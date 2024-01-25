#pragma once
#include "card.h"
#include "hand.h"
#include "range.h"

namespace pokerbot {

struct Auctioneer {

  Auctioneer();

  int get_bid(const std::vector<Range>& ranges, const std::vector<Card>& board, const Hand& hand,
              float time_budget);
};

}  // namespace pokerbot
