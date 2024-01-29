#pragma once
#include "game.h"

namespace pokerbot {

class TimeManager {
 public:
  TimeManager() : num_actions_taken_(0) {}

  void update_on_get_action(const GameInfo& /*game_info*/) { num_actions_taken_++; }

  double time_ms_allowed_for_cfr(const GameInfo& game_info) const {
    // Estimate the number of actions we have to take per hand before having a good enough sample
    double avg_num_actions_per_hand =
        num_actions_taken_ < 50 ? 6 : static_cast<double>(num_actions_taken_) / game_info.hand_num;
    double num_actions_left_match = avg_num_actions_per_hand * game_info.num_hands_left_in_match();

    // Add epsilon to make sure we don't bust the limit
    double time_left_seconds = game_info.game_clock + 0.5;
    return (1000.0 * time_left_seconds) / num_actions_left_match;
  }

 private:
  unsigned num_actions_taken_;
};

}  // namespace pokerbot
