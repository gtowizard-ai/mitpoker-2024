#pragma once
#include "auction.h"
#include "cfr.h"
#include "game.h"
#include "states.h"

namespace pokerbot {

class MainBot {
 public:
  MainBot();
  /*
    Called when a new hand starts. Called NUM_HANDS times.

    @param active Your player's index.
  */
  void handle_new_hand(const GameInfo& game_info, const RoundStatePtr& round_state, int active);

  /*
    Called when a hand ends. Called NUM_HANDS times.

    @param active Your player's index.
  */
  void handle_hand_over(const GameInfo& game_info, const TerminalStatePtr& terminal_state,
                        int active);

  /*
    Where the magic happens - your code should implement this function.
    Called any time the engine needs an action from your bot.

    @param active Your player's index.
    @return Your action.
  */
  Action get_action(const GameInfo& game_info, const RoundStatePtr& state, int active);

 private:
  const Game game_;
  Auctioneer auctioneer_;
  std::array<Range, 2> ranges_;
  CFR cfr_;
  // TODO
  // HandActionsValues preflop_sb_cached_strategy_;
};

}  // namespace pokerbot
