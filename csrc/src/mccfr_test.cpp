#include "mccfr.h"
#include <gtest/gtest.h>

using namespace pokerbot;

class MCCFRTest : public ::testing::Test {
 protected:
  Game game_;
};

TEST_F(MCCFRTest, TestNoError) {
  auto mccfr = MCCFR(100);

  std::vector<Range> ranges = {Range(), Range()};
  for (int i = 0; i < 2; i++) {
    for (unsigned index = 0; index < ranges[i].num_hands(); index++) {
      ranges[i].range[index] /= ranges[i].num_hands();
    }
  }
  std::array<std::optional<int>, 2> bids = {std::nullopt, std::nullopt};
  std::array<int, 2> pips = {SMALL_BLIND, BIG_BLIND};
  std::array<int, 2> stacks = {STARTING_STACK - SMALL_BLIND, STARTING_STACK - BIG_BLIND};
  std::array<std::array<std::string, 3>, 2> hands;
  int active = 0;
  std::array<std::string, 5> deck = {"Kc", "Td", "7h", "3s", "2c"};
  std::vector<std::string> cards = {"2h", "2d"};
  hands[active][0] = cards[0];
  hands[active][1] = cards[1];
  auto round_state = std::make_shared<RoundState>(0, round::RIVER, false, bids, pips, stacks,
                                                  std::move(hands), std::move(deck), nullptr);
  const auto timer_start = std::chrono::high_resolution_clock::now();
  mccfr.solve(ranges, std::static_pointer_cast<const RoundState>(round_state), 0, 100);
  const auto timer_end = std::chrono::high_resolution_clock::now();
  std::cout
      << "Time Taken: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start).count()
      << " ms.\n";
}

TEST_F(MCCFRTest, TestNutAirToyGame) {
  // Test Classic Nuts+Air vs. Bluff catcher toy game.
  auto mccfr = MCCFR(100);
  std::vector<Range> ranges = {Range(), Range()};
  for (int i = 0; i < 2; i++) {
    for (unsigned index = 0; index < ranges[i].num_hands(); index++) {
      ranges[i].range[index] = 0;
    }
  }
  ranges[1].range[Hand("ThTs5s").index()] = 1;
  ranges[1].range[Hand("ThTc5s").index()] = 1;
  ranges[1].range[Hand("ThTd5s").index()] = 1;
  ranges[1].range[Hand("TsTc5s").index()] = 1;
  ranges[1].range[Hand("TsTd5s").index()] = 1;
  ranges[1].range[Hand("TcTd5s").index()] = 1;

  ranges[0].range[Hand("4h4s").index()] = 1;
  ranges[0].range[Hand("4h4c").index()] = 1;
  ranges[0].range[Hand("4h4d").index()] = 1;
  ranges[0].range[Hand("4s4c").index()] = 1;
  ranges[0].range[Hand("4s4d").index()] = 1;
  ranges[0].range[Hand("4c4d").index()] = 1;

  ranges[0].range[Hand("AhAs").index()] = 1;
  ranges[0].range[Hand("AhAc").index()] = 1;
  ranges[0].range[Hand("AhAd").index()] = 1;
  ranges[0].range[Hand("AsAc").index()] = 1;
  ranges[0].range[Hand("AsAd").index()] = 1;
  ranges[0].range[Hand("AcAd").index()] = 1;

  int active = 0;
  std::array<std::string, 5> deck = {"2c", "2d", "2h", "3s", "3c"};
  std::array<std::optional<int>, 2> bids = {0, 0};
  std::array<int, 2> pips = {SMALL_BLIND, BIG_BLIND};
  std::array<int, 2> stacks = {200, 50};
  std::array<std::array<std::string, 3>, 2> hands;

  std::vector<std::string> cards = {"4h", "4d"};
  hands[active][0] = cards[0];
  hands[active][1] = cards[1];
  auto round_state = std::make_shared<RoundState>(0, round::RIVER, false, bids, pips, stacks,
                                                  std::move(hands), std::move(deck), nullptr);

  const std::vector<card_t> board_cards = round_state->board_cards();

  ranges[1].to_3_cards_range(Game(), board_cards);

  const auto timer_start = std::chrono::high_resolution_clock::now();
  auto strategy =
      mccfr.solve(ranges, std::static_pointer_cast<const RoundState>(round_state), 0, 100);
  const auto timer_end = std::chrono::high_resolution_clock::now();
  std::cout
      << "Time Taken: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start).count()
      << " ms.\n";
}