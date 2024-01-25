#pragma once

#include <vector>
#include "hand_category.h"
#include "range.h"

namespace pokerbot {

struct HandStrength {
  int index;
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

class TerminalValue {
 public:
  TerminalValue(const Game& game, const std::vector<card_t>& board,
                const std::vector<Range>& ranges, unsigned player_id);

  /// Computes the expected payoff of the river nodes
  void compute_terminal_values(const Payoff& payoff);

 private:
  static constexpr int num_players_ = 2;
  const Game& game_;
  const std::vector<card_t>& board_;
  const std::vector<Range>& ranges_;
  const unsigned player_id_;
  double sum_;
  std::vector<float> terminal_values_;

  std::array<std::vector<HandStrength>, num_players_> player_strengthses_;
};

}  // namespace pokerbot