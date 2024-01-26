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
    return data[hand * num_actions_ + action];
  }

  float const& operator()(const hand_t hand, const unsigned action) const {
    return data[hand * num_actions_ + action];
  }

  // Store a 2D data of [action, hand] in 1D vector of size num_hands * num_actions
  std::vector<float> data;

  unsigned num_hands_{};
  unsigned num_actions_{};
};

// Fixme: We won't create one MCCFR object for each Decision. Better to create one object and
// Reuse it over for each Round
class MCCFR {
 public:
  explicit MCCFR(unsigned warm_up_iterations);
  HandActionsValues solve(const std::array<Range, 2>& ranges, const RoundStatePtr& round_state,
                          unsigned player_id, float time_budget_ms);

  // Actions considered at the state
  const auto& legal_actions() const { return available_actions_; }

  auto num_actions() const { return available_actions_.size(); }

 private:
  void build_tree(const RoundStatePtr& round_state);
  [[nodiscard]] float get_child_value(unsigned hand, unsigned action) const;
  void update_root_value(unsigned hand);
  void update_root_value();
  void update_regrets(const std::array<Range, 2>& ranges);
  HandActionsValues get_last_strategy();
  [[nodiscard]] float get_linear_cfr_discount_factor(unsigned hand) const;
  void initial_regrets();
  void precompute_child_values(const std::array<Range, 2>& ranges,
                               const RoundStatePtr& round_state);

  static constexpr unsigned max_available_actions_ = 10;
  static constexpr long long time_checkpoints_ = 1000;
  static constexpr float timer_error_bound_ = 0.85;

  std::minstd_rand random_generator_;
  unsigned warm_up_iterations_;
  std::array<std::vector<float>, max_available_actions_> children_values_{};

  unsigned num_hands_;
  unsigned player_id_;

  unsigned my_contribution_;

  // The value of the root node - size = num_hands_
  std::array<float, NUM_HANDS_POSTFLOP_3CARDS> values_;
  // Temporary buffer to store sum of regrets
  // std::array<double, NUM_HANDS_POSTFLOP_3CARDS> sum_buffer_;
  std::array<unsigned, NUM_HANDS_POSTFLOP_3CARDS> num_steps_;

  std::vector<Action> available_actions_;

  // Regrets for root node. Indexed by [action].
  HandActionsValues regrets_;
};

}  // namespace pokerbot