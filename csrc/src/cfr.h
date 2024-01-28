#pragma once
#include <vector>
#include "equity.h"
#include "game.h"
#include "range.h"
#include "states.h"

namespace pokerbot {

class HandActionsValues {
 public:
  explicit HandActionsValues(unsigned num_hands, unsigned num_actions, float value);

  HandActionsValues() = default;

  float& operator()(const hand_t hand, const unsigned action) {
    return data[hand * num_actions_ + action];
  }

  float const& operator()(const hand_t hand, const unsigned action) const {
    return data[hand * num_actions_ + action];
  }

  // Store a 2D data of [action, hand] in 1D vector of size num_hands * num_actions
  std::vector<float> data;

  unsigned num_hands_ = 0;
  unsigned num_actions_ = 0;
};

// NB: We won't create one CFR object for each Decision. Better to create one object and
// Reuse it over for each Round
class CFR {
 public:
  explicit CFR(const Game& game);

  HandActionsValues solve(const std::array<Range, 2>& ranges, const RoundStatePtr& state,
                          unsigned player_id, float time_budget_ms, unsigned max_num_iters = 1000);

  // Actions considered at the state
  const auto& legal_actions() const { return actions_; }

  auto num_actions() const { return actions_.size(); }

 private:
  void step(const std::array<Range, 2>& ranges);

  void build_tree();

  [[nodiscard]] float get_linear_cfr_discount_factor() const;

  void precompute_cfvs_fixed_nodes(const std::array<Range, 2>& ranges);

  void compute_node_cfvs(const Range& traverser_range, const Range& opponent_range,
                         const Payoff& payoff, std::vector<float>& cfvs) const;

  void update_opponent_cfvs_vs_bet();
  void update_opponent_regrets();
  void update_opponent_reaches(const Range& opponent_range);
  void update_opponent_strategy();

  void update_hero_reaches(const Range& hero_range);
  void update_hero_regrets();
  void update_hero_cfvs_bet_node();
  void update_hero_strategy();

  static constexpr unsigned max_available_actions_ = 3;

  const Game& game_;

  std::array<std::vector<float>, max_available_actions_> children_values_{};

  RoundStatePtr root_;
  std::array<hand_t, 2> num_hands_{0, 0};
  unsigned player_id_;
  unsigned num_steps_ = 0;

  // Actions in abstraction at root node
  std::vector<Action> actions_;

  // Regrets for root node. Indexed by [action].
  HandActionsValues regrets_;
  HandActionsValues opponent_regrets_vs_bet_;
  HandActionsValues strategy_;
  // Since we only allow fold/call vs. raise for our opponent,
  // we can only keep track of fold strategy
  std::vector<float> opponent_fold_strategy_vs_bet_;

  Range hero_range_raise_;
  Range opponent_range_raise_fold_;
  Range opponent_range_raise_call_;
  std::vector<float> raise_fold_cfvs_;
  std::vector<float> raise_call_cfvs_;
};

}  // namespace pokerbot
