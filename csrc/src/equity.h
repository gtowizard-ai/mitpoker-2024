#pragma once

#include "poker_hand.h"
#include "range.h"

#include <type_traits>

namespace pokerbot {

struct Payoff {
  float win;
  float lose;
};

/// Computes the counterfactual values and adds them to the `cfvs` vector.
/// @param cfvs A vector of size `hero_range.num_hands()`. Note that the results are added to this
/// vector, not overwritten. The type `T` must be a floating-point type.
/// @param is_river_equity If `true`, the `cfvs` vector will be converted to a vector of equities.
/// In this case, the `payoff` must be 0.5.
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
void compute_cfvs_river(const Game& game, const Range& hero_range, const Range& opponent_range,
                        const PokerHand& board, std::vector<T>& cfvs, const Payoff& payoff,
                        bool is_river_equity = false);

/// Computes the equities of the hero range against the opponent range.
/// @return A vector of size `hero_range.num_hands()`.
std::vector<float> compute_equities(const Game& game, const Range& hero_range,
                                    const Range& opponent_range,
                                    const std::vector<card_t>& board_cards);

}  // namespace pokerbot
