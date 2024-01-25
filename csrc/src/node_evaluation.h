#pragma once

#include <vector>
#include "hand_category.h"
#include "range.h"

namespace pokerbot {

struct HandStrength {
  unsigned index;
  strength_t strength;

  bool operator<(const HandStrength& a) const { return strength < a.strength; }
};

struct Payoff {
  float lose;
  float tie;
  float win;
};

struct OutcomeProb {
  double lose;
  double tie;
  double win;
};

// FIXME: Move to corresponding class
void compute_terminal_values(const Game& game, const std::vector<card_t>& board,
                             const Payoff& payoff, const std::vector<Range>& ranges,
                             unsigned player_id);

}  // namespace pokerbot