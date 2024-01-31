#pragma once
#include <array>
#include "game.h"
#include "ranges_utils.h"

namespace pokerbot {

class TimeManager {
 public:
  static constexpr unsigned ROUNDS = 4;
  static constexpr int WARM_UP_TIME = 6;

  explicit TimeManager(const std::array<float, ROUNDS>& ratio = {0.15, 0.20, 0.25, 0.40})
      : ratio_(ratio), last_hand_in_round_({-1, -1, -1, -1}) {}

  void update_action(const GameInfo& game_info, const RoundStatePtr& state) {
    const auto round = state->round().id;

    // Do not count timer for the cases where we do not need timing
    const bool bid_action = ranges::contains(state->legal_actions(), Action::Type::BID);
    if (state->legal_actions().size() == 1 || bid_action) {
      return;
    }

    if (last_hand_in_round_[round] != game_info.hand_num) {
      last_hand_in_round_[round] = game_info.hand_num;
      hands_played_per_round_[round]++;
    }
    total_actions_per_round_[round]++;

    if (total_actions_per_round_[round] == 1) {
      unused_resources_ -= ratio_[round];
      unused_rounds_ = std::max(1, unused_rounds_ - 1);
    }
  }

  [[nodiscard]] float get_time_budget_ms(const GameInfo& game_info,
                                         const RoundStatePtr& state) const {
    // Do not output timer for the cases where we do not need timing
    const bool bid_action = ranges::contains(state->legal_actions(), Action::Type::BID);
    if (state->legal_actions().size() == 1 || bid_action) {
      return 1;
    }

    const unsigned round = state->round().id;
    if (game_info.hand_num < 20) {
      return WARM_UP_TIME;
    }

    const float game_time =
        (std::max(game_info.game_clock - 0.5, 0.0) * 1000.0) / game_info.num_hands_left_in_match();

    // Formula : Assign the game time based on the ratio
    // If some rounds (like river) never reach, use their ratio in previous rounds
    // Devide the time of each round over possible number of actions in that round
    return std::max(game_time *
                        (ratio_[round] + unused_resources_ / static_cast<float>(unused_rounds_)) *
                        static_cast<float>(hands_played_per_round_[round]) /
                        (static_cast<float>(total_actions_per_round_[round])),
                    1.0f);
  }

 private:
  float unused_resources_ = 1;
  int unused_rounds_ = ROUNDS;
  std::array<unsigned, ROUNDS> total_actions_per_round_{};
  std::array<float, ROUNDS> ratio_;

  std::array<int, ROUNDS> hands_played_per_round_{};
  std::array<int, ROUNDS> last_hand_in_round_;
};

}  // namespace pokerbot
