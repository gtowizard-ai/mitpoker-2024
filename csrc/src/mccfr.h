#pragma once
#include "game.h"
#include "states.h"
#include <vector>
#include <random>

namespace pokerbot {

using Regrets = std::vector<float>;
using Strategy = std::vector<float>;
using Range = std::vector<float>;

class MCCFR {
public:
  MCCFR(const GameInfo& game_state, const RoundStatePtr& round_state, int player_id, const std::vector<Range>& ranges);
  void update_regrets();
  float get_child_value(int action, int hand);
  float MCCFR::get_child_value(int action);
  void update_sum_strategies(float strat_discount);
  void MCCFR::compute_avg_strategy();
  void MCCFR::step();


private:
  const GameInfo& game_;
  const RoundStatePtr& round_state_;
  const int player_;
  const std::vector<Range> ranges_;

  // The value of the node
  float value_;

  std::mt19937 random_generator_;  // Mersenne Twister engine

  std::vector<Action> available_actions_;

  // Regrets for root node. Indexed by [action].
  Regrets regrets_;

  // Strategy for the root node. Indexed by [action].
  Strategy sum_strategies_;
  Strategy last_strategies_;
  Strategy average_strategies_;

  // Temporary buffer to store sum of strategies
  double sum_buffer_;

  int num_steps_;
};

} // namespace pokerbot