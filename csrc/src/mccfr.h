#pragma once
#include <random>
#include <vector>
#include "game.h"
#include "range.h"
#include "states.h"

namespace pokerbot {

class HandActionsValues {
 public:
  explicit HandActionsValues(unsigned num_hands, unsigned num_actions);

  HandActionsValues() = default;

  float& operator()(const hand_t hand, const unsigned action) {
    return data[action * num_hands_ + hand];
  }

  float const& operator()(const hand_t hand, const unsigned action) const {
    return data[action * num_hands_ + hand];
  }

  // Store a 2D data of [hand, action] in 1D vector of size num_hands * num_actions
  std::vector<float> data;

  // Number of hands
  unsigned num_hands_{};
};

// Fixme: We won't create one MCCFR object for each Decision. Better to create one object and
// Reuse it over for each Round
class MCCFR {
 public:
  MCCFR(const GameInfo& game_state, unsigned warm_up_iterations);
  void build_tree(const RoundStatePtr& round_state);
  float get_child_value(unsigned hand, unsigned action);
  float get_child_value(unsigned action);
  void update_root_value();
  void update_regrets(const std::vector<Range>& ranges);
  HandActionsValues get_last_strategy();
  [[nodiscard]] float get_linear_cfr_discount_factor(unsigned hand) const;
  void initial_regrets();
  void step(const std::vector<Range>& ranges);
  void solve(const std::vector<Range>& ranges, const RoundStatePtr& round_state, unsigned player_id,
             std::chrono::microseconds time_budget);

 private:
  static constexpr unsigned max_available_actions_ = 10;

  const GameInfo& game_;
  std::mt19937 random_generator_;  // Mersenne Twister engine
  unsigned warm_up_iterations_;

  unsigned num_hands_;
  unsigned player_id_;

  // The value of the root node - size = num_hands_
  std::array<float, NUM_HANDS_POSTFLOP_3CARDS> values_;
  // Temporary buffer to store sum of regrets
  std::array<double, NUM_HANDS_POSTFLOP_3CARDS> sum_buffer_;
  std::array<unsigned, NUM_HANDS_POSTFLOP_3CARDS> num_steps_;

  std::vector<Action> available_actions_;

  // Regrets for root node. Indexed by [action].
  HandActionsValues regrets_;
};

}  // namespace pokerbot