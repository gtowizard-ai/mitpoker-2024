#pragma once
#include <vector>
#include "equity.h"
#include "game.h"
#include "range.h"
#include "states.h"

namespace pokerbot {

struct HandActionsValues {
  explicit HandActionsValues(unsigned num_actions, unsigned num_hands, float value)
      : data(num_actions, std::vector<float>(ceil_to_multiple(num_hands), value)),
        num_actions(num_actions),
        num_hands(num_hands) {}

  HandActionsValues() = default;

  float& operator()(const unsigned action, const hand_t hand) { return data[action][hand]; }

  float const& operator()(const unsigned action, const hand_t hand) const {
    return data[action][hand];
  }

  // Store a 2D data of [action, hand]
  std::vector<std::vector<float>> data;
  unsigned num_actions = 0;
  unsigned num_hands = 0;
};

// NB: Should create this object once and reuse it each time you need to solve a subgame
class CFR {
 public:
  explicit CFR(const Game& game);

  void solve(const std::array<Range, 2>& ranges, const RoundStatePtr& state, unsigned player_id,
             float time_budget_ms, std::optional<Action> force_raise_root = std::nullopt,
             unsigned max_num_iters = 1000);

  // Actions considered at the state
  const auto& legal_actions() const { return actions_; }

  auto num_actions() const { return actions_.size(); }

  // Strategy at root
  const auto& strategy() const { return strategy_; }

 private:
  void step(const std::array<Range, 2>& ranges);

  void build_tree(std::optional<Action> force_raise_root);

  [[nodiscard]] float get_linear_cfr_discount_factor() const;

  void precompute_cfvs_fixed_nodes(const std::array<Range, 2>& ranges);

  void compute_fold_cfvs(const Range& traverser_range, const Range& opponent_range, float payoff,
                         std::vector<float>& cfvs) const;
  void compute_showdown_cfvs(const Range& traverser_range, const Range& opponent_range,
                             float payoff, std::vector<float>& cfvs) const;

  void update_opponent_cfvs_vs_bet();
  void update_opponent_regrets();
  void update_opponent_reaches(const Range& opponent_range);
  void update_opponent_strategy();

  void update_hero_reaches(const Range& hero_range);
  void update_hero_regrets();
  void update_hero_cfvs_bet_node();
  void update_hero_strategy();

  static constexpr unsigned MAX_LEGAL_ACTIONS = 3;

  const Game& game_;

  // CFVs for each action at root node
  std::array<std::vector<float>, MAX_LEGAL_ACTIONS> children_cfvs_{};

  RoundStatePtr root_;
  // Number of hands of each player, based on whether they have 2 or 3 cards
  std::array<hand_t, 2> num_hands_{0, 0};
  // Index of player at root node
  unsigned player_id_;
  // Number of steps/iterations completed,
  // where we update both players on each iteration
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

  mutable BoardDataCache board_data_cache_;
};

}  // namespace pokerbot
