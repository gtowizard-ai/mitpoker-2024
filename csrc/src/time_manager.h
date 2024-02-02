#pragma once
#include "game.h"
#include "states.h"

#include <array>

namespace pokerbot {

class TimeManager {
 public:
  static constexpr unsigned ROUNDS = 4;
  static constexpr float WARM_UP_TIME = 6;  // in milliseconds
  static constexpr int WARM_UP_NUM_HANDS = 20;
  static constexpr int TOTAL_ACTIONS_MARGIN = 3;

  explicit TimeManager(const std::array<float, ROUNDS>& ratio = {0.1, 0.3, 0.3, 0.3})
      : ratio_(ratio) {}

  void update_action(const RoundState& state) {
    const auto round_id = state.round().id;
    total_actions_per_round_[round_id]++;
  }

  [[nodiscard]] float get_time_budget_ms(const GameInfo& game_info, const RoundState& state) const {
    if (game_info.hand_num < WARM_UP_NUM_HANDS) {
      return WARM_UP_TIME;
    }

    const unsigned round_id = state.round().id;

    float expected_sum = 0.0;
    for (unsigned i = 0; i < ROUNDS; ++i) {
      expected_sum += ratio_[i] * (total_actions_per_round_[i] + TOTAL_ACTIONS_MARGIN);
    }

    expected_sum *= static_cast<float>(game_info.num_hands_left_in_match()) /
                    static_cast<float>(game_info.hand_num);

    const float remaining_time_ms =
        (game_info.game_clock - (0.5f - 0.4f / game_info.num_hands_left_in_match())) * 1000.0f;

    return std::max(ratio_[round_id] * remaining_time_ms / expected_sum, 1.0f);
  }

 private:
  std::array<float, ROUNDS> ratio_;
  std::array<unsigned, ROUNDS> total_actions_per_round_{};
};

}  // namespace pokerbot
