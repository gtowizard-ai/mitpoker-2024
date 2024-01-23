#pragma once

#include <memory>

namespace pokerbots::skeleton {

struct GameInfo {
  int bankroll;
  double game_clock;
  int round_num;

  GameInfo(int bankroll, double game_clock, int round_num)
      : bankroll(bankroll), game_clock(game_clock), round_num(round_num) {}
};

using GameInfoPtr = std::shared_ptr<const GameInfo>;

}  // namespace pokerbots::skeleton
