#include "actions.h"
#include <gtest/gtest.h>

using namespace pokerbot;

TEST(ActionsTest, NonSenseTest) {
  auto action = Action(Action::Type::RAISE, 5);
  ASSERT_EQ(action.amount, 5);
}
