#include "states.h"
#include <gtest/gtest.h>

#include "ranges_utils.h"

using namespace pokerbot;

class StatesTest : public ::testing::Test {
 protected:
  std::array<std::array<std::string, 3>, 2> dummy_hands = {
      std::array<std::string, 3>{"Ac", "Ad", "As"},
      std::array<std::string, 3>{"Kc", "Kh", ""},
  };
};

TEST_F(StatesTest, TestGlobalState) {
  auto round_state =
      std::make_shared<RoundState>(0, false, EMPTY_BIDS, BLINDS, STARTING_STACKS, dummy_hands);

  ASSERT_EQ(round_state->effective_stack(), STARTING_STACK - BIG_BLIND);
  ASSERT_EQ(round_state->effective_stack_start_round(), STARTING_STACK);
  ASSERT_EQ(round_state->pot(), SMALL_BLIND + BIG_BLIND);
  ASSERT_EQ(round_state->pot_start_round(), 0);
  ASSERT_EQ(round_state->spent_total(SB_POS), SMALL_BLIND);
  ASSERT_EQ(round_state->spent_total(BB_POS), BIG_BLIND);
  ASSERT_EQ(round_state->legal_actions().size(), 3);
  ASSERT_TRUE(ranges::contains(round_state->legal_actions(), Action::Type::FOLD));
  ASSERT_TRUE(ranges::contains(round_state->legal_actions(), Action::Type::CALL));
  ASSERT_TRUE(ranges::contains(round_state->legal_actions(), Action::Type::RAISE));
  ASSERT_EQ(round_state->round(), round::PREFLOP);
  ASSERT_EQ(round_state->raise_bounds()[0], 2 * BIG_BLIND);
  ASSERT_EQ(round_state->raise_bounds()[1], STARTING_STACK);
}
