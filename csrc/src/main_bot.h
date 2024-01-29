#pragma once
#include <random>
#include "auction.h"
#include "cfr.h"
#include "game.h"
#include "states.h"
#include "time_manager.h"

namespace pokerbot {

class MainBot {
 public:
  MainBot();

  ///  Called when a new hand starts. Called NUM_HANDS times.
  ///  @param active Your player's index.
  void handle_new_hand(const GameInfo& game_info, const RoundStatePtr& round_state, int active);

  /// Called when a hand ends. Called NUM_HANDS times.
  /// @param active Your player's index.
  void handle_hand_over(const GameInfo& game_info, const TerminalStatePtr& terminal_state,
                        int active);

  /// Called any time the engine needs an action from your bot.
  /// @param active Your player's index.
  /// @return Your action.
  Action get_action(const GameInfo& game_info, const RoundStatePtr& state, int active);

 private:
  const Game game_;
  Auctioneer auctioneer_;
  std::array<Range, 2> ranges_;
  CFR cfr_;
  TimeManager time_manager_;
  mutable std::mt19937 gen_;
  // TODO Cache preflop strategies
  // HandActionsValues preflop_sb_cached_strategy_;

  // Sample action based on strategy in `cfr_`
  // Won't sample any action with prob < `min_prob_sampling`
  // Then update hero's range based on sampled action
  Action sample_action_and_update_range(const RoundState& state, const Hand& hand, int hero_id,
                                        float min_prob_sampling = 0.01);
};

}  // namespace pokerbot
