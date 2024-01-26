#include "main_bot.h"
#include <gtest/gtest.h>

using namespace pokerbot;

std::array<int, 2> STARTING_STACKS = {STARTING_STACK, STARTING_STACK};
std::array<int, 2> STARTING_BETS = {SMALL_BLIND, BIG_BLIND};
std::array<int, 2> NO_BETS = {0, 0};
std::array<std::optional<int>, 2> NO_BIDS = {std::nullopt, std::nullopt};

TEST(MainBotTest, TestPreflopState) {
  MainBot bot;

  GameInfo game_info(0, 0.0, 1);
  std::array<std::array<std::string, 3>, 2> empty_hand = {
      {{"2c", "2d", ""}, {"", "", ""}},
  };
  StatePtr round_state =
      std::make_shared<RoundState>(0, false, NO_BIDS, STARTING_BETS, STARTING_STACKS, empty_hand,
                                   std::vector<card_t>{}, nullptr);

  auto action =
      bot.get_action(game_info, std::static_pointer_cast<const RoundState>(round_state), 0);
  ASSERT_EQ(action.action_type, Action::Type::CALL);
  // TODO more tests
}

TEST(MainBotTest, TestRiverState) {
  MainBot bot;

  GameInfo game_info(0, 0.0, 1);
  std::array<std::array<std::string, 3>, 2> empty_hand = {
      {{"4c", "4d", "5c"}, {"", "", ""}},
  };
  const auto board_cards = Card::to_vector("2c2d2h2s3c");
  StatePtr round_state = std::make_shared<RoundState>(0, false, NO_BIDS, NO_BETS, STARTING_STACKS,
                                                      empty_hand, board_cards, nullptr);

  auto action =
      bot.get_action(game_info, std::static_pointer_cast<const RoundState>(round_state), 0);
  ASSERT_TRUE(action.action_type == Action::Type::CHECK ||
              action.action_type == Action::Type::RAISE);
  // TODO more tests
}
