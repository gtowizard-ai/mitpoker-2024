#pragma once

namespace pokerbot {

struct GameInfo {
  int bankroll;
  double game_clock;
  int round_num;

  GameInfo(int bankroll, double game_clock, int round_num)
      : bankroll(bankroll), game_clock(game_clock), round_num(round_num) {}
};

}  // namespace pokerbot
