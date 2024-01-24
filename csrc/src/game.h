#pragma once
#include "hand.h"

namespace pokerbot {

struct GameInfo {
  int bankroll;
  double game_clock;
  int hand_num;

  GameInfo(int bankroll, double game_clock, int hand_num)
      : bankroll(bankroll), game_clock(game_clock), hand_num(hand_num) {}
};

// Number of private cards that is supported
enum class NumCards {
  Two,
  Three,
};

class Game {
 public:
  Game() {
    two_cards_hands_.assign(NUM_HANDS_POSTFLOP_2CARDS, Hand());
    for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
      for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
        Hand hand(i, j);
        two_cards_hands_[hand.index()] = hand;
      }
    }

    three_cards_hands_.assign(NUM_HANDS_POSTFLOP_3CARDS, Hand());
    for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
      for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
        for (card_t k = j + 1; k < MAX_DECK_SIZE; ++k) {
          Hand hand(i, j, k);
          three_cards_hands_[hand.index()] = hand;
        }
      }
    }
  }

  static auto num_hands(NumCards num_cards) {
    return num_cards == NumCards::Two ? NUM_HANDS_POSTFLOP_2CARDS : NUM_HANDS_POSTFLOP_3CARDS;
  }

  const auto& hands(NumCards num_cards) const {
    return num_cards == NumCards::Two ? two_cards_hands_ : three_cards_hands_;
  }

 private:
  std::vector<Hand> two_cards_hands_;
  std::vector<Hand> three_cards_hands_;
};

}  // namespace pokerbot
