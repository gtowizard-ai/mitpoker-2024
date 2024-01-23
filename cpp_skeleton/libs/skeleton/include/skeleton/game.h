#pragma once

#include <memory>

namespace pokerbots::skeleton {

struct GameInfo {
  int bankroll;
  double gameClock;
  int roundNum;

  GameInfo(int bankroll, double gameClock, int roundNum)
      : bankroll(bankroll), gameClock(gameClock),
        roundNum(roundNum) {}
};

using GameInfoPtr = std::shared_ptr<const GameInfo>;

} // namespace pokerbots::skeleton
