#include "preflop_equity.h"

namespace pokerbot {

std::vector<float> compute_preflop_cfvs(const Range& opponent_range, const Payoff& payoff) {
  if (opponent_range.num_cards != NumCards::Two) {
    throw std::invalid_argument("Can't call compute_preflop_cfvs with 3-cards range");
  }
  if (std::abs(payoff.tie) > 1e-4 || std::abs(payoff.win + payoff.lose) > 1e-4) {
    throw std::invalid_argument("Preflop - Only support payoffs of tie=0, lose=-win");
  }

  std::array<float, NUM_HANDS_PREFLOP> pf_range{};
  for (hand_t i = 0; i < NUM_HANDS_POSTFLOP_2CARDS; ++i) {
    pf_range[PREFLOP_HAND_IDX[i]] += opponent_range.range[i];
  }

  std::array<float, NUM_HANDS_PREFLOP> pf_cfvs{};

  for (hand_t i = 0; i < NUM_HANDS_PREFLOP; ++i) {
    for (hand_t j = 0; j < NUM_HANDS_PREFLOP; ++j) {
      pf_cfvs[i] += PREFLOP_PAYOFFS[i * NUM_HANDS_PREFLOP + j] * pf_range[j] * payoff.win;
    }
  }

  std::vector<float> postflop_cfvs;
  postflop_cfvs.reserve(NUM_HANDS_POSTFLOP_2CARDS);
  for (hand_t i = 0; i < NUM_HANDS_POSTFLOP_2CARDS; ++i) {
    postflop_cfvs.emplace_back(pf_cfvs[PREFLOP_HAND_IDX[i]]);
  }

  return postflop_cfvs;
}

}  // namespace pokerbot