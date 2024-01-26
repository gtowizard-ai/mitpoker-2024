#pragma once

#include "poker_hand.h"
#include "range.h"

namespace pokerbot {

/// Computes the counterfactual values and stores them in `cfvs`.
void compute_cfvs_river(const Game& game, const Range& hero_range, const Range& opponent_range,
                        const PokerHand& board, std::vector<double>& cfvs, double payoff);

/// Computes the equities of the hero range against the opponent range.
/// @return A vector of size `hero_range.num_hands()`.
std::vector<float> compute_equities(const Game& game, const Range& hero_range,
                                    const Range& opponent_range,
                                    const std::vector<card_t>& board_cards);

}  // namespace pokerbot
