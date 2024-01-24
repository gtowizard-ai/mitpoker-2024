#pragma once

namespace pokerbot {

struct GameInfo {
  int bankroll;
  double game_clock;
  int hand_num;

  GameInfo(int bankroll, double game_clock, int hand_num)
      : bankroll(bankroll), game_clock(game_clock), hand_num(hand_num) {}
};

}  // namespace pokerbot
