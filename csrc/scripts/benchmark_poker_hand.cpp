#include "../src/poker_hand.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

using namespace gtowizard;

int main() {
  constexpr unsigned num_trials = 1000;
  constexpr unsigned num_hands = 1000000;

  std::array<card_t, 52> deck;
  std::iota(deck.begin(), deck.end(), 0);

  std::vector<card_t> cards;
  cards.reserve(num_hands * 7);

  std::mt19937_64 rng;  // fixed seed

  for (unsigned i = 0; i < num_hands; ++i) {
    std::sample(deck.cbegin(), deck.cend(), std::back_inserter(cards), 7, rng);
    std::shuffle(cards.begin() + i * 7, cards.begin() + (i + 1) * 7, rng);
  }

  strength_t strength_sum = 0;
  const auto start = std::chrono::high_resolution_clock::now();

  for (unsigned i = 0; i < num_trials; ++i) {
    for (unsigned j = 0; j < num_hands; ++j) {
      PokerHand hand{cards[j * 7],     cards[j * 7 + 1], cards[j * 7 + 2], cards[j * 7 + 3],
                     cards[j * 7 + 4], cards[j * 7 + 5], cards[j * 7 + 6]};
      strength_sum += hand.evaluate();
    }
  }

  const auto end = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> duration = end - start;

  std::cout << "7-card hand evaluation benchmark\n";
  std::cout << (num_trials * num_hands) / duration.count() << " [eval/s]\n";
  std::cout << "strength_sum = " << strength_sum << "\n";
}
