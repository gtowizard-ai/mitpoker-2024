#include "preflop_equity.h"
#include <gtest/gtest.h>
#include <cmath>

#include "hand.h"
#include "ranges_utils.h"

using namespace pokerbot;

TEST(PreflopEquityTest, TestIntegrityValues) {
  ASSERT_EQ(PREFLOP_HAND_IDX.size(), NUM_HANDS_POSTFLOP_2CARDS);
  ASSERT_TRUE(std::all_of(PREFLOP_HAND_IDX.begin(), PREFLOP_HAND_IDX.end(), [](auto idx) {
    return std::isfinite(idx) && idx >= 0 && idx < NUM_HANDS_PREFLOP;
  }));

  ASSERT_EQ(PREFLOP_2_CARDS_PAYOFFS.size(), NUM_HANDS_PREFLOP * NUM_HANDS_PREFLOP);
  ASSERT_TRUE(std::all_of(
      PREFLOP_2_CARDS_PAYOFFS.begin(), PREFLOP_2_CARDS_PAYOFFS.end(),
      [](auto payoff) { return std::isfinite(payoff) && payoff >= -1.0 && payoff <= 1.0; }));

  auto idx_22 = PREFLOP_HAND_IDX[Hand("2c2d").index()];
  auto idx_32 = PREFLOP_HAND_IDX[Hand("3c2d").index()];
  auto idx_aa = PREFLOP_HAND_IDX[Hand("AcAd").index()];

  ASSERT_NEAR(PREFLOP_2_CARDS_PAYOFFS[idx_22 * NUM_HANDS_PREFLOP + idx_22], 0, 1e-4);
  ASSERT_NEAR(PREFLOP_2_CARDS_PAYOFFS[idx_32 * NUM_HANDS_PREFLOP + idx_32], 0, 1e-4);
  ASSERT_NEAR(PREFLOP_2_CARDS_PAYOFFS[idx_aa * NUM_HANDS_PREFLOP + idx_aa], 0, 1e-4);
  ASSERT_NEAR(PREFLOP_2_CARDS_PAYOFFS[idx_22 * NUM_HANDS_PREFLOP + idx_32], 0.5 * (0.6352 - 0.3036),
              1e-4);
  ASSERT_NEAR(PREFLOP_2_CARDS_PAYOFFS[idx_aa * NUM_HANDS_PREFLOP + idx_32], 0.8695 - 0.1253, 1e-4);
}

TEST(PreflopEquityTest, TestComputeCFVs) {
  Range opponent_range;

  std::vector<float> cfvs(NUM_HANDS_POSTFLOP_2CARDS, 0);
  compute_cfvs_preflop(opponent_range, 1.0, cfvs);

  ASSERT_EQ(cfvs.size(), NUM_HANDS_POSTFLOP_2CARDS);
  ASSERT_NEAR(cfvs[Hand("2c2d").index()], cfvs[Hand("2c2h").index()], 1e-6);
  ASSERT_NEAR(cfvs[Hand("2c2d").index()], cfvs[Hand("2c2s").index()], 1e-6);
  ASSERT_NEAR(cfvs[Hand("2d2h").index()], cfvs[Hand("2d2s").index()], 1e-6);
  ASSERT_NEAR(cfvs[Hand("2d2h").index()], cfvs[Hand("2h2s").index()], 1e-6);
  ASSERT_NEAR(cfvs[Hand("2d2s").index()], cfvs[Hand("2h2s").index()], 1e-6);

  ASSERT_NEAR(cfvs[Hand("3c2d").index()], cfvs[Hand("3s2h").index()], 1e-6);
  ASSERT_NEAR(cfvs[Hand("AsKs").index()], cfvs[Hand("AhKh").index()], 1e-6);

  ASSERT_NEAR(cfvs[Hand("2d3h").index()], *std::min_element(cfvs.begin(), cfvs.end()), 1e-6);
  ASSERT_NEAR(cfvs[Hand("AsAh").index()], *std::max_element(cfvs.begin(), cfvs.end()), 1e-6);

  // FIXME NEED MORE TESTS HERE
}
