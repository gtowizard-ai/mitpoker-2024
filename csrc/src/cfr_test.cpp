#include "cfr.h"
#include <fmt/ranges.h>
#include <gtest/gtest.h>
#include "ranges_utils.h"

using namespace pokerbot;

class CFRTest : public ::testing::Test {
 protected:
  Game game_;
};

TEST_F(CFRTest, TestNoErrorThreeVsThreeCards) {
  auto cfr = CFR(game_);

  const auto board_cards = Card::to_vector("KcTd7h3s2c");
  std::array<Range, 2> ranges{Range(), Range()};
  ranges[0].update_on_board_cards(game_, board_cards);
  ranges[0].to_3_cards_range(game_, board_cards);
  ranges[1] = ranges[0];

  std::array<std::string, 2> hands = {"2h2d2s", "8h8d8s"};

  auto round_state = std::make_shared<RoundState>(BB_POS, false, EMPTY_BIDS, BLINDS,
                                                  STARTING_STACKS, hands, board_cards, nullptr);

  cfr.solve(ranges, round_state, 0, 10);
}

TEST_F(CFRTest, TestRiverNutAirToyGame) {
  // Test Classic Nuts+Air vs. Bluff catcher toy game.
  const auto board_cards = Card::to_vector("2c2d2h3c3d");
  auto cfr = CFR(game_);
  std::array<Range, 2> ranges{Range(), Range()};
  for (int i = 0; i < 2; i++) {
    std::fill(ranges[i].range.begin(), ranges[i].range.end(), 0);
  }

  ranges[0].update_on_board_cards(game_, board_cards);
  ranges[1].update_on_board_cards(game_, board_cards);
  ranges[1].to_3_cards_range(game_, board_cards);

  ranges[1].range[Hand("ThTs5s").index()] = 1;
  ranges[1].range[Hand("ThTc5s").index()] = 1;
  ranges[1].range[Hand("ThTd5s").index()] = 1;
  ranges[1].range[Hand("TsTc5s").index()] = 1;
  ranges[1].range[Hand("TsTd5s").index()] = 1;
  ranges[1].range[Hand("TcTd5s").index()] = 1;

  const std::vector<std::string> bluffs = {"4h4s", "4h4c"};
  for (const auto& bluff : bluffs) {
    ranges[0].range[Hand(bluff).index()] = 1;
  }
  const std::vector<std::string> value_bets = {"AhAs", "AhAc"};
  for (const auto& value_bet : value_bets) {
    ranges[0].range[Hand(value_bet).index()] = 1;
  }

  const int hero_id = 0;
  std::array<std::optional<int>, 2> bids = {10, 5};
  std::array<int, 2> bets = {0, 0};
  std::array<int, 2> stacks = {STARTING_STACK - 100, STARTING_STACK - 95};
  std::array<std::string, 2> hands;
  hands[hero_id] = "4h4c";

  auto round_state =
      std::make_shared<RoundState>(BB_POS, false, bids, bets, stacks, hands, board_cards, nullptr);

  cfr.solve(ranges, std::static_pointer_cast<const RoundState>(round_state), hero_id, 4000, 500);
  const auto& strategy = cfr.strategy();

  fmt::print("Board is {} \n", Card::to_string(board_cards));
  double bluff_frequency = 0;
  for (const auto& bluff : bluffs) {
    fmt::print("{} Check = {} / Raise = {} \n", bluff, strategy(Hand(bluff).index(), 0),
               strategy(Hand(bluff).index(), 1));
    bluff_frequency += strategy(Hand(bluff).index(), 1);
  }
  double valuebet_frequency = 0;
  for (const auto& value_bet : value_bets) {
    fmt::print("{} Check = {} / Raise = {} \n", value_bet, strategy(Hand(value_bet).index(), 0),
               strategy(Hand(value_bet).index(), 1));
    valuebet_frequency += strategy(Hand(value_bet).index(), 1);
  }
  double pot_odds = (double)round_state->effective_stack() /
                    (round_state->effective_stack() + round_state->pot());

  double ratio_bluff_to_vbet = bluff_frequency / valuebet_frequency;
  fmt::print("Pot odds are {:.4f} | Ratio bluff/valuebet = {:.4f}\n", pot_odds,
             ratio_bluff_to_vbet);
  ASSERT_NEAR(valuebet_frequency, 2.0, 1e-4);
  ASSERT_NEAR(pot_odds, ratio_bluff_to_vbet, 0.05);
}

TEST_F(CFRTest, TestPreflopOpenRaiseStrategy) {
  auto cfr = CFR(game_);

  const auto board_cards = Card::to_vector("");
  std::array<Range, 2> ranges{Range(), Range()};

  std::array<std::string, 2> hands = {"", "8h8d"};

  auto round_state = std::make_shared<RoundState>(SB_POS, false, EMPTY_BIDS, BLINDS,
                                                  STARTING_STACKS, hands, board_cards, nullptr);

  cfr.solve(ranges, round_state, 0, 500);

  const auto& strategy = cfr.strategy();

  for (std::string hand_str : {"AcAs", "AcKc", "AcKd", "AcTh", "Ac2d", "6c6d", "2c2d", "JdTh",
                               "Tc9h", "Tc8d", "9s8s", "9h8s", "6c5c", "3c2c", "3c2d"}) {
    Hand hand(hand_str);
    fmt::print("{} Fold = {} / Call = {} / Raise = {} \n", hand.to_string(),
               strategy(hand.index(), 0), strategy(hand.index(), 1), strategy(hand.index(), 2));
  }

  // TODO Add tests/asserts here
}
