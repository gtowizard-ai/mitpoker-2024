#include "mccfr.h"
#include <gtest/gtest.h>

using namespace pokerbot;

class MCCFRTest : public ::testing::Test {
 protected:
  Game game_;
};

TEST_F(MCCFRTest, TestNoError) {
  auto mccfr = MCCFR(100);

  const std::vector<Range> ranges = {Range(), Range()};
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
  mccfr.solve(ranges, std::static_pointer_cast<const RoundState>(round_state), 0,
              static_cast<std::chrono::milliseconds>(100));
}