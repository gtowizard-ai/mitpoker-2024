#pragma once
#include <array>
#include "game.h"

namespace pokerbot {

class TimeManager {
 public:
  TimeManager() = default;

  void update_action(const RoundStatePtr& state) {
    total_actions_per_round_[state->round().id]++;
    total_actions_++;
  }

  void update_time(const RoundStatePtr& state, const float time_elapsed_ms) {
    if (total_actions_ < 50 && time_elapsed_ms > WARM_UP_TIME) {
      total_time_ms_per_round_[state->round().id] += WARM_UP_TIME;
      return;
    }
    total_time_ms_per_round_[state->round().id] += time_elapsed_ms;
  }

  [[nodiscard]] float get_time_budget_ms(const GameInfo& game_info,
                                         const RoundStatePtr& state) const {
    const unsigned round = state->round().id;
    if (game_info.hand_num < 50) {
      return WARM_UP_TIME;
    }

    float passed_time = 0;
    for (const auto time_ms : total_time_ms_per_round_) {
      passed_time += time_ms;
    }

    const float game_time =
        (std::max(game_info.game_clock - 0.5, 0.0) * 1000.0) / game_info.num_hands_left_in_match();
    return std::max(game_time * total_time_ms_per_round_[round] *
                        static_cast<float>(game_info.hand_num) /
                        (total_actions_per_round_[round] * passed_time),
                    1.0f);
  }

 private:
  static constexpr unsigned ROUNDS = 4;
  static constexpr int WARM_UP_TIME = 6;
  unsigned total_actions_ = 0;

 public:
  std::array<float, ROUNDS> total_time_ms_per_round_{};
  std::array<unsigned, ROUNDS> total_actions_per_round_{};
};

}  // namespace pokerbot
