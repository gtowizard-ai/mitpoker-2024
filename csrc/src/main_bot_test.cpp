#include "main_bot.h"
#include <fmt/ranges.h>
#include <gtest/gtest.h>

using namespace pokerbot;

std::array<int, 2> NO_BETS = {0, 0};
std::array<std::optional<int>, 2> NO_BIDS = {std::nullopt, std::nullopt};

TEST(MainBotTest, TestPreflopState) {
  MainBot bot;
  GameInfo game_info(0, 0.0, 1);

  for (auto hand : std::vector<std::array<std::string, 3>>{
           {"Ac", "Ad", ""}, {"9s", "8s", ""}, {"2c", "3c", ""}, {"2d", "7c", ""}}) {
    std::array<std::array<std::string, 3>, 2> hands = {
        {hand, {"", "", ""}},
    };
    StatePtr round_state = std::make_shared<RoundState>(0, false, NO_BIDS, BLINDS, STARTING_STACKS,
                                                        hands, std::vector<card_t>{}, nullptr);

    auto action =
        bot.get_action(game_info, std::static_pointer_cast<const RoundState>(round_state), 0);
    ASSERT_TRUE(action.type == Action::Type::FOLD || action.type == Action::Type::CALL ||
                action.type == Action::Type::RAISE);
    fmt::print("{} - {} \n", hands[0], action.to_string());
  }

  // TODO more tests
}

TEST(MainBotTest, TestFlopState) {
  MainBot bot;

  GameInfo game_info(0, 0.0, 1);
  std::array<std::array<std::string, 3>, 2> empty_hand = {
      {{"4c", "4d"}, {"", "", ""}},
  };
  const auto board_cards = Card::to_vector("2c2d2h");
  StatePtr round_state = std::make_shared<RoundState>(0, false, NO_BIDS, NO_BETS, STARTING_STACKS,
                                                      empty_hand, board_cards, nullptr);

  auto action =
      bot.get_action(game_info, std::static_pointer_cast<const RoundState>(round_state), 0);
  ASSERT_TRUE(action.type == Action::Type::CHECK || action.type == Action::Type::RAISE);
  // TODO more tests
}

TEST(MainBotTest, TestTurnState) {
  MainBot bot;

  GameInfo game_info(0, 0.0, 1);
  std::array<std::array<std::string, 3>, 2> empty_hand = {
      {{"4c", "4d"}, {"", "", ""}},
  };
  const auto board_cards = Card::to_vector("2c2d2h2s");
  StatePtr round_state = std::make_shared<RoundState>(0, false, NO_BIDS, NO_BETS, STARTING_STACKS,
                                                      empty_hand, board_cards, nullptr);

  auto action =
      bot.get_action(game_info, std::static_pointer_cast<const RoundState>(round_state), 0);
  ASSERT_TRUE(action.type == Action::Type::CHECK || action.type == Action::Type::RAISE);
  // TODO more tests
}

TEST(MainBotTest, TestRiverState) {
  MainBot bot;

  GameInfo game_info(0, 0.0, 1);
  std::array<std::array<std::string, 3>, 2> empty_hand = {
      {{"4c", "4d", "5c"}, {"", "", ""}},
  };
  std::array<std::optional<int>, 2> bids = {0, 0};
  const auto board_cards = Card::to_vector("2c2d2h2s3c");
  StatePtr round_state = std::make_shared<RoundState>(0, false, bids, NO_BETS, STARTING_STACKS,
                                                      empty_hand, board_cards, nullptr);

  auto action =
      bot.get_action(game_info, std::static_pointer_cast<const RoundState>(round_state), 0);
  ASSERT_TRUE(action.type == Action::Type::CHECK || action.type == Action::Type::RAISE);
  // TODO more tests
}
