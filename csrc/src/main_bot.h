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
  std::optional<HandActionsValues> hero_sb_cached_strategy_;
  std::optional<HandActionsValues> opp_sb_cached_strategy_;
  std::optional<std::vector<Action>> hero_sb_cached_legal_actions_;
  std::optional<std::vector<Action>> opp_sb_cached_legal_actions_;

  // We update our opponent's range with a small weight of uniform random range
  // As a regularization since our estimated ranges are not that great
  static constexpr float WEIGHT_UNIFORM_RANDOM_RANGE_OPPONENT = 0.05;

  // Sample action based on strategy in `cfr_`
  // Won't sample any action with prob < `min_prob_sampling`
  Action sample_action(const GameInfo& game_info, const RoundState& state, const Hand& hand,
                       int hero_id, const HandActionsValues& strategy,
                       const std::vector<Action>& legal_actions, float min_prob_sampling = 0.05);

  void update_range(int player, const HandActionsValues& strategy,
                    const std::vector<Action>& legal_actions, const Action& action,
                    bool is_hero_node);

  Action get_action_any_player(const GameInfo& game_info, const RoundStatePtr& state, int player,
                               std::optional<Action> sampled_action);
};

}  // namespace pokerbot
