#include "terminal_value.h"
#include <gtest/gtest.h>
#include <string>

using namespace pokerbot;

class TerminalValueTest : public ::testing::Test {
 protected:
  Game game_;
};

TEST_F(TerminalValueTest, TestNoError) {
  const std::vector<Range> ranges = {Range(), Range()};
  const std::string board = "KcTd7h3s2c";
  TerminalValue terminal(game_, Card::to_vector(board), ranges, 0);
  const Payoff payoff = {100, 10, -100};
  EXPECT_NO_THROW(terminal.compute_terminal_values(payoff));
}
