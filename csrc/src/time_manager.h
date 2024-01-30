#pragma once
#include <array>
#include "game.h"

namespace pokerbot {

class TimeManager {
 public:
  static constexpr unsigned ROUNDS = 4;
  static constexpr int WARM_UP_TIME = 6;

  explicit TimeManager(const std::array<float, ROUNDS>& ratio = {0.15, 0.20, 0.25, 0.40})
      : ratio_(ratio), last_hand_in_round_({-1, -1, -1, -1}) {}

  void update_action(const GameInfo& game_info, const RoundStatePtr& state) {
    const auto round = state->round().id;
    if (last_hand_in_round_[round] != game_info.hand_num) {
      last_hand_in_round_[round] = game_info.hand_num;
      games_played_per_round_[round]++;
    }
    total_actions_per_round_[state->round().id]++;
  }

  [[nodiscard]] float get_time_budget_ms(const GameInfo& game_info,
                                         const RoundStatePtr& state) const {
    const unsigned round = state->round().id;
    if (game_info.hand_num < 20) {
      return WARM_UP_TIME;
    }

    const float game_time =
        (std::max(game_info.game_clock - 0.5, 0.0) * 1000.0) / game_info.num_hands_left_in_match();

    return std::max(game_time * ratio_[round] * static_cast<float>(games_played_per_round_[round]) /
                        (static_cast<float>(total_actions_per_round_[round])),
                    1.0f);
  }

 private:
  std::array<unsigned, ROUNDS> total_actions_per_round_{};
  std::array<float, ROUNDS> ratio_;

  std::array<int, ROUNDS> games_played_per_round_{};
  std::array<int, ROUNDS> last_hand_in_round_;
};

}  // namespace pokerbot
