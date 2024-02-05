#include "states.h"
#include <fmt/format.h>
#include <gtest/gtest.h>
#include "card.h"
#include "ranges_utils.h"

using namespace gtowizard;

class StatesTest : public ::testing::Test {
 protected:
  std::array<std::string, 2> dummy_hands = {"AcAdAs", "KcKh"};

  static void assert_fold_call_raise_allowed(const std::vector<Action::Type>& actions) {
    ASSERT_EQ(actions.size(), 3);
    ASSERT_TRUE(ranges::contains(actions, Action::Type::FOLD));
    ASSERT_TRUE(ranges::contains(actions, Action::Type::CALL));
    ASSERT_TRUE(ranges::contains(actions, Action::Type::RAISE));
  }

  static void assert_fold_call_allowed(const std::vector<Action::Type>& actions) {
    ASSERT_EQ(actions.size(), 2);
    ASSERT_TRUE(ranges::contains(actions, Action::Type::FOLD));
    ASSERT_TRUE(ranges::contains(actions, Action::Type::CALL));
  }

  static void assert_check_raise_allowed(const std::vector<Action::Type>& actions) {
    ASSERT_EQ(actions.size(), 2);
    ASSERT_TRUE(ranges::contains(actions, Action::Type::CHECK));
    ASSERT_TRUE(ranges::contains(actions, Action::Type::RAISE));
  }
};

TEST_F(StatesTest, TestPreflopStates) {
  auto round_state =
      std::make_shared<RoundState>(SB_POS, false, EMPTY_BIDS, BLINDS, STARTING_STACKS, dummy_hands);

  ASSERT_EQ(round_state->effective_stack(), STARTING_STACK - BIG_BLIND);
  ASSERT_EQ(round_state->effective_stack_start_round(), STARTING_STACK);
  ASSERT_EQ(round_state->pot(), SMALL_BLIND + BIG_BLIND);
  ASSERT_EQ(round_state->pot_start_round(), 0);
  ASSERT_EQ(round_state->spent_total(SB_POS), SMALL_BLIND);
  ASSERT_EQ(round_state->spent_total(BB_POS), BIG_BLIND);
  assert_fold_call_raise_allowed(round_state->legal_actions());
  ASSERT_EQ(round_state->round(), round::PREFLOP);
  ASSERT_EQ(round_state->raise_bounds()[0], 2 * BIG_BLIND);
  ASSERT_EQ(round_state->raise_bounds()[1], STARTING_STACK);

  auto limp_state =
      std::static_pointer_cast<const RoundState>(round_state->proceed(Action{Action::Type::CALL}));

  ASSERT_EQ(limp_state->effective_stack(), STARTING_STACK - BIG_BLIND);
  ASSERT_EQ(limp_state->effective_stack_start_round(), STARTING_STACK);
  ASSERT_EQ(limp_state->pot(), BIG_BLIND + BIG_BLIND);
  ASSERT_EQ(limp_state->pot_start_round(), 0);
  ASSERT_EQ(limp_state->spent_total(SB_POS), BIG_BLIND);
  ASSERT_EQ(limp_state->spent_total(BB_POS), BIG_BLIND);
  assert_check_raise_allowed(limp_state->legal_actions());
  ASSERT_EQ(limp_state->round(), round::PREFLOP);
  ASSERT_EQ(limp_state->raise_bounds()[0], 2 * BIG_BLIND);
  ASSERT_EQ(limp_state->raise_bounds()[1], STARTING_STACK);

  auto raise_state = std::static_pointer_cast<const RoundState>(
      round_state->proceed(Action{Action::Type::RAISE, 3 * BIG_BLIND}));

  ASSERT_EQ(raise_state->effective_stack(), STARTING_STACK - 3 * BIG_BLIND);
  ASSERT_EQ(raise_state->effective_stack_start_round(), STARTING_STACK);
  ASSERT_EQ(raise_state->pot(), 4 * BIG_BLIND);
  ASSERT_EQ(raise_state->pot_start_round(), 0);
  ASSERT_EQ(raise_state->spent_total(SB_POS), 3 * BIG_BLIND);
  ASSERT_EQ(raise_state->spent_total(BB_POS), BIG_BLIND);
  assert_fold_call_raise_allowed(raise_state->legal_actions());
  ASSERT_EQ(raise_state->round(), round::PREFLOP);
  ASSERT_EQ(raise_state->raise_bounds()[0], 5 * BIG_BLIND);
  ASSERT_EQ(raise_state->raise_bounds()[1], STARTING_STACK);

  auto raise_raise_state = std::static_pointer_cast<const RoundState>(
      raise_state->proceed(Action{Action::Type::RAISE, 10 * BIG_BLIND}));

  ASSERT_EQ(raise_raise_state->effective_stack(), STARTING_STACK - 10 * BIG_BLIND);
  ASSERT_EQ(raise_raise_state->effective_stack_start_round(), STARTING_STACK);
  ASSERT_EQ(raise_raise_state->pot(), 13 * BIG_BLIND);
  ASSERT_EQ(raise_raise_state->pot_start_round(), 0);
  ASSERT_EQ(raise_raise_state->spent_total(SB_POS), 3 * BIG_BLIND);
  ASSERT_EQ(raise_raise_state->spent_total(BB_POS), 10 * BIG_BLIND);
  assert_fold_call_raise_allowed(raise_raise_state->legal_actions());
  ASSERT_EQ(raise_raise_state->round(), round::PREFLOP);
  ASSERT_EQ(raise_raise_state->raise_bounds()[0], 17 * BIG_BLIND);
  ASSERT_EQ(raise_raise_state->raise_bounds()[1], STARTING_STACK);
}

TEST_F(StatesTest, TestFlopStatesPreAuction) {
  auto round_state =
      std::make_shared<RoundState>(BB_POS, false, EMPTY_BIDS, std::array<int, 2>{0, 0},
                                   std::array<int, 2>{STARTING_STACK - 3, STARTING_STACK - 3},
                                   dummy_hands, Card::to_vector("AcKdTh"));

  ASSERT_EQ(round_state->effective_stack(), STARTING_STACK - 3);
  ASSERT_EQ(round_state->effective_stack_start_round(), STARTING_STACK - 3);
  ASSERT_EQ(round_state->pot(), 6);
  ASSERT_EQ(round_state->pot_start_round(), 6);
  ASSERT_EQ(round_state->spent_total(SB_POS), 3);
  ASSERT_EQ(round_state->spent_total(BB_POS), 3);
  ASSERT_EQ(round_state->bets[SB_POS], 0);
  ASSERT_EQ(round_state->bets[BB_POS], 0);
  assert_check_raise_allowed(round_state->legal_actions());
  ASSERT_EQ(round_state->round(), round::FLOP);
  ASSERT_EQ(round_state->raise_bounds()[0], BIG_BLIND);
  ASSERT_EQ(round_state->raise_bounds()[1], STARTING_STACK - 3);
}

TEST_F(StatesTest, TestFlopStatesPostAuction) {
  std::array<std::optional<int>, 2> bids{2, 5};
  auto round_state =
      std::make_shared<RoundState>(BB_POS, false, bids, std::array<int, 2>{0, 0},
                                   std::array<int, 2>{STARTING_STACK - 3, STARTING_STACK - 5},
                                   dummy_hands, Card::to_vector("AcKdTh"));

  ASSERT_EQ(round_state->effective_stack(), STARTING_STACK - 5);
  ASSERT_EQ(round_state->effective_stack_start_round(), STARTING_STACK - 5);
  ASSERT_EQ(round_state->pot(), 8);
  ASSERT_EQ(round_state->pot_start_round(), 8);
  ASSERT_EQ(round_state->spent_total(SB_POS), 3);
  ASSERT_EQ(round_state->spent_total(BB_POS), 5);
  ASSERT_EQ(round_state->bets[SB_POS], 0);
  ASSERT_EQ(round_state->bets[BB_POS], 0);
  assert_check_raise_allowed(round_state->legal_actions());
  ASSERT_EQ(round_state->round(), round::FLOP);
  ASSERT_EQ(round_state->raise_bounds()[0], BIG_BLIND);
  ASSERT_EQ(round_state->raise_bounds()[1], STARTING_STACK - 5);

  auto check_state =
      std::static_pointer_cast<const RoundState>(round_state->proceed(Action{Action::Type::CHECK}));
  ASSERT_EQ(check_state->effective_stack(), STARTING_STACK - 5);
  ASSERT_EQ(check_state->effective_stack_start_round(), STARTING_STACK - 5);
  ASSERT_EQ(check_state->pot(), 8);
  ASSERT_EQ(check_state->pot_start_round(), 8);
  ASSERT_EQ(check_state->spent_total(SB_POS), 3);
  ASSERT_EQ(check_state->spent_total(BB_POS), 5);
  ASSERT_EQ(check_state->bets[SB_POS], 0);
  ASSERT_EQ(check_state->bets[BB_POS], 0);
  assert_check_raise_allowed(check_state->legal_actions());
  ASSERT_EQ(check_state->round(), round::FLOP);
  ASSERT_EQ(check_state->raise_bounds()[0], BIG_BLIND);
  ASSERT_EQ(check_state->raise_bounds()[1], STARTING_STACK - 5);

  auto raise_state = std::static_pointer_cast<const RoundState>(
      round_state->proceed(Action{Action::Type::RAISE, 10}));
  ASSERT_EQ(raise_state->effective_stack(), STARTING_STACK - 15);
  ASSERT_EQ(raise_state->effective_stack_start_round(), STARTING_STACK - 5);
  ASSERT_EQ(raise_state->pot(), 18);
  ASSERT_EQ(raise_state->pot_start_round(), 8);
  ASSERT_EQ(raise_state->spent_total(SB_POS), 3);
  ASSERT_EQ(raise_state->spent_total(BB_POS), 15);
  ASSERT_EQ(raise_state->bets[SB_POS], 0);
  ASSERT_EQ(raise_state->bets[BB_POS], 10);
  assert_fold_call_raise_allowed(raise_state->legal_actions());
  ASSERT_EQ(raise_state->round(), round::FLOP);
  ASSERT_EQ(raise_state->raise_bounds()[0], 20);
  ASSERT_EQ(raise_state->raise_bounds()[1], STARTING_STACK - 5);

  auto allin_state = std::static_pointer_cast<const RoundState>(
      round_state->proceed(Action{Action::Type::RAISE, STARTING_STACK - 5}));

  ASSERT_EQ(allin_state->effective_stack(), 0);
  ASSERT_EQ(allin_state->effective_stack_start_round(), STARTING_STACK - 5);
  ASSERT_EQ(allin_state->pot(), STARTING_STACK + 3);
  ASSERT_EQ(allin_state->pot_start_round(), 8);
  ASSERT_EQ(allin_state->spent_total(SB_POS), 3);
  ASSERT_EQ(allin_state->spent_total(BB_POS), STARTING_STACK);
  ASSERT_EQ(allin_state->bets[SB_POS], 0);
  ASSERT_EQ(allin_state->bets[BB_POS], STARTING_STACK - 5);
  assert_fold_call_allowed(allin_state->legal_actions());
  ASSERT_EQ(allin_state->round(), round::FLOP);
  ASSERT_EQ(allin_state->raise_bounds()[0], STARTING_STACK - 5);
  ASSERT_EQ(allin_state->raise_bounds()[1], STARTING_STACK - 5);
}
