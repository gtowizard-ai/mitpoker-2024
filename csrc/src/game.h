#pragma once
#include "hand.h"

namespace pokerbot {

struct GameInfo {
  // Total number of chips you've gained or lost from the beginning of the match to the start of this round
  int bankroll;
  // Total number of seconds your bot has left to play this game
  double game_clock;
  // Hand from 1 to `num_hands_in_match`
  int hand_num;
  // Number of total hands that we get to play against our opponent
  int num_hands_in_match;

  GameInfo(int bankroll, double game_clock, int hand_num, int num_hands_in_match)
      : bankroll(bankroll),
        game_clock(game_clock),
        hand_num(hand_num),
        num_hands_in_match(num_hands_in_match) {}
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
    subhand_indices_.assign(NUM_HANDS_POSTFLOP_3CARDS, std::array<hand_t, 3>{});
    for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
      for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
        for (card_t k = j + 1; k < MAX_DECK_SIZE; ++k) {
          Hand hand(i, j, k);
          auto& subhand_indices = subhand_indices_[hand.index()];
          three_cards_hands_[hand.index()] = hand;
          subhand_indices[0] = Hand(i, j).index();
          subhand_indices[1] = Hand(i, k).index();
          subhand_indices[2] = Hand(j, k).index();
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

  const auto& subhand_indices() const { return subhand_indices_; }

 private:
  std::vector<Hand> two_cards_hands_;
  std::vector<Hand> three_cards_hands_;
  std::vector<std::array<hand_t, 3>> subhand_indices_;
};

}  // namespace pokerbot
