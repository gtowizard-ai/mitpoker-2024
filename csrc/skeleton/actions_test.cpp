#include <gtest/gtest.h>
#include "actions.h"

using namespace pokerbots::skeleton;

TEST(ActionsTest, NonSenseTest) {
  auto action = Action(Action::Type::RAISE, 5);
  ASSERT_EQ(action.amount, 5);
}
