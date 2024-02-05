#include "../src/equity.h"

#include <chrono>
#include <iostream>
#include <random>

using namespace gtowizard;

int main() {
  constexpr unsigned num_trials_2_vs_3 = 10000;
  constexpr unsigned num_trials_3_vs_2 = 10000;
  constexpr unsigned num_trials_3_vs_3 = 5000;

  Range range_2_cards;
  Range range_3_cards;

  std::mt19937_64 rng;  // fixed seed
  std::uniform_real_distribution<float> dist;

  for (unsigned i = 0; i < NUM_HANDS_POSTFLOP_2CARDS; ++i) {
    range_2_cards.range[i] = dist(rng);
  }

  range_3_cards.num_cards = NumCards::Three;
  for (unsigned i = 0; i < NUM_HANDS_POSTFLOP_3CARDS; ++i) {
    range_3_cards.range[i] = dist(rng);
  }

  const Game game;
  const auto board = Card::to_vector("Qs9c6d3d2h");

  range_2_cards.update_on_board_cards(game, board);
  range_3_cards.update_on_board_cards(game, board);

  const auto start_2_vs_3 = std::chrono::high_resolution_clock::now();

  for (unsigned i = 0; i < num_trials_2_vs_3; ++i) {
    compute_equities(game, range_2_cards, range_3_cards, board);
  }

  const auto end_2_vs_3 = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> duration_2_vs_3 = end_2_vs_3 - start_2_vs_3;

  std::cout << "2 vs 3 cards benchmark\n";
  std::cout << num_trials_2_vs_3 / duration_2_vs_3.count() << " [calls/s]" << std::endl;

  const auto start_3_vs_2 = std::chrono::high_resolution_clock::now();

  for (unsigned i = 0; i < num_trials_3_vs_2; ++i) {
    compute_equities(game, range_3_cards, range_2_cards, board);
  }

  const auto end_3_vs_2 = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> duration_3_vs_2 = end_3_vs_2 - start_3_vs_2;

  std::cout << "3 vs 2 cards benchmark\n";
  std::cout << num_trials_3_vs_2 / duration_3_vs_2.count() << " [calls/s]" << std::endl;

  const auto start_3_vs_3 = std::chrono::high_resolution_clock::now();

  for (unsigned i = 0; i < num_trials_3_vs_3; ++i) {
    compute_equities(game, range_3_cards, range_3_cards, board);
  }

  const auto end_3_vs_3 = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> duration_3_vs_3 = end_3_vs_3 - start_3_vs_3;

  std::cout << "3 vs 3 cards benchmark\n";
  std::cout << num_trials_3_vs_3 / duration_3_vs_3.count() << " [calls/s]" << std::endl;
}
