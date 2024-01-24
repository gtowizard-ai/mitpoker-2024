#pragma once
#include "game.h"
#include "states.h"
#include <vector>
#include <random>
#include "range.h"

namespace pokerbot {

// Indexed by [hand * num_actions + action]
class Regret {
public:
  explicit Regret(unsigned num_hands, unsigned num_actions);

  Regret() = default;

  float& operator()(const hand_t hand, const unsigned action) {
    return data[action * num_hands_ + hand];
  }

  float const& operator()(const hand_t hand, const unsigned action) const {
    return data[action * num_hands_ + hand];
  }

  // Indexed by [action * num_hands + hand]
  std::vector<float> data;

  // Number of hands
  unsigned num_hands_{};
};


// Fixme: We won't create one MCCFR object for each Decision. Better to create one object and
// Reuse it over for each Round
class MCCFR {
public:
  MCCFR(const GameInfo& game_state, const RoundStatePtr& round_state, unsigned player_id, const std::vector<Range>& ranges);
  std::vector<float> get_root_value();
  void update_regrets();
  float get_child_value(unsigned hand, unsigned action);
  float get_child_value(unsigned action);
  Regret get_avg_strategy();
  [[nodiscard]] float get_discount_factors(unsigned hand) const;
  void step();

private:
  const GameInfo& game_;
  const RoundStatePtr& round_state_;
  const unsigned player_;
  const std::vector<Range> ranges_;
  const unsigned num_hands_;

  // The value of the node
  std::vector<float> values_;

  std::mt19937 random_generator_;  // Mersenne Twister engine

  std::vector<Action> available_actions_;

  // Regrets for root node. Indexed by [action].
  Regret regrets_;

  // Temporary buffer to store sum of strategies
  std::vector<double> sum_buffer_;

  std::vector<unsigned> num_steps_;
};

} // namespace pokerbot