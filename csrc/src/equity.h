#pragma once

#include "range.h"

#include <optional>
#include <type_traits>

namespace gtowizard {

class BoardDataCache {
 public:
  explicit BoardDataCache(const Game& game) : game_(game) {}
  const std::vector<hand_t>& get_possible_hands_2_cards(const std::vector<card_t>& board_cards);
  const std::vector<hand_t>& get_possible_hands_3_cards(const std::vector<card_t>& board_cards);
  const std::vector<uint32_t>& get_sorted_hands_2_cards(const std::vector<card_t>& board_cards);
  const std::vector<uint32_t>& get_sorted_hands_3_cards(const std::vector<card_t>& board_cards);

 private:
  void check_board(const std::vector<card_t>& board_cards);
  void sort_hands(std::vector<uint32_t>& hands);

  const Game& game_;
  std::vector<card_t> cached_board_;
  std::optional<std::vector<hand_t>> possible_hands_2_cards_;
  std::optional<std::vector<hand_t>> possible_hands_3_cards_;
  std::optional<std::vector<uint32_t>> sorted_hands_2_cards_;
  std::optional<std::vector<uint32_t>> sorted_hands_3_cards_;
};

/// Computes the counterfactual values with a fixed payoff and stores them in `cfvs`.
/// @param board_cards The board cards. Required to zero out the CFVs of impossible hands.
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
void compute_cfvs_fixed_payoff(const Game& game, const Range& hero_range,
                               const Range& opponent_range, const std::vector<card_t>& board_cards,
                               std::vector<T>& cfvs, T payoff, BoardDataCache& cache);

/// Computes the counterfactual values and adds them to the `cfvs` vector.
/// @param cfvs A vector of size `hero_range.num_hands()`. Note that the results are added to this
/// vector, not overwritten. The type `T` must be a floating-point type.
/// @param is_river_equity If `true`, the `cfvs` vector will be converted to a vector of equities.
/// In this case, the `payoff` must be 0.5.
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
void compute_cfvs_showdown(const Game& game, const Range& hero_range, const Range& opponent_range,
                           const std::vector<card_t>& board_cards, std::vector<T>& cfvs, T payoff,
                           BoardDataCache& cache, bool is_river_equity = false);

/// Computes the equities of the hero range against the opponent range.
/// @return A vector of size `hero_range.num_hands()`.
std::vector<float> compute_equities(const Game& game, const Range& hero_range,
                                    const Range& opponent_range,
                                    const std::vector<card_t>& board_cards);

}  // namespace gtowizard
