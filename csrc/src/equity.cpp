#include "equity.h"

#include <hwy/contrib/sort/vqsort.h>

#include <stdexcept>

namespace pokerbot {

static std::vector<double> compute_weights_2_vs_3(const Game& game, const Range& opponent_range) {
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  const auto& opponent_hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  for (unsigned i = 0; i < opponent_hands.size(); ++i) {
    const auto& cards = opponent_hands[i].cards;
    const auto& subs = subhand_indices[i];
    const double prob = opponent_range.range[i];

    opponent_sum_0 += prob;
    opponent_sum_1[cards[0]] += prob;
    opponent_sum_1[cards[1]] += prob;
    opponent_sum_1[cards[2]] += prob;
    opponent_sum_2[subs[0]] += prob;
    opponent_sum_2[subs[1]] += prob;
    opponent_sum_2[subs[2]] += prob;
  }

  const auto& hero_hands = game.hands(NumCards::Two);
  std::vector<double> weights;
  weights.reserve(hero_hands.size());

  for (unsigned i = 0; i < hero_hands.size(); ++i) {
    const auto& cards = hero_hands[i].cards;
    const double weight =
        opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] + opponent_sum_2[i];
    weights.push_back(weight);
  }

  return weights;
}

static std::vector<double> compute_weights_3_vs_2(const Game& game, const Range& opponent_range) {
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);

  const auto& opponent_hands = game.hands(NumCards::Two);

  for (unsigned i = 0; i < opponent_hands.size(); ++i) {
    const auto& cards = opponent_hands[i].cards;
    const double prob = opponent_range.range[i];

    opponent_sum_0 += prob;
    opponent_sum_1[cards[0]] += prob;
    opponent_sum_1[cards[1]] += prob;
  }

  const auto& hero_hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  std::vector<double> weights;
  weights.reserve(hero_hands.size());

  for (unsigned i = 0; i < hero_hands.size(); ++i) {
    const auto& cards = hero_hands[i].cards;
    const auto& subs = subhand_indices[i];
    const double weight = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] -
                          opponent_sum_1[cards[2]] + opponent_range.range[subs[0]] +
                          opponent_range.range[subs[1]] + opponent_range.range[subs[2]];
    weights.push_back(weight);
  }

  return weights;
}

static std::vector<double> compute_weights_3_vs_3(const Game& game, const Range& opponent_range) {
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  const auto& hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  for (unsigned i = 0; i < hands.size(); ++i) {
    const auto& cards = hands[i].cards;
    const auto& subs = subhand_indices[i];
    const double prob = opponent_range.range[i];

    opponent_sum_0 += prob;
    opponent_sum_1[cards[0]] += prob;
    opponent_sum_1[cards[1]] += prob;
    opponent_sum_1[cards[2]] += prob;
    opponent_sum_2[subs[0]] += prob;
    opponent_sum_2[subs[1]] += prob;
    opponent_sum_2[subs[2]] += prob;
  }

  std::vector<double> weights;
  weights.reserve(hands.size());

  for (unsigned i = 0; i < hands.size(); ++i) {
    const auto& cards = hands[i].cards;
    const auto& subs = subhand_indices[i];
    const double weight = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] -
                          opponent_sum_1[cards[2]] + opponent_sum_2[subs[0]] +
                          opponent_sum_2[subs[1]] + opponent_sum_2[subs[2]] -
                          opponent_range.range[i];
    weights.push_back(weight);
  }

  return weights;
}

// Assume that probs of hands that are blocked by board cards are properly set to zero
static std::vector<double> compute_weights(const Game& game, const Range& hero_range,
                                           const Range& opponent_range) {
  if (hero_range.num_cards == NumCards::Two) {
    return compute_weights_2_vs_3(game, opponent_range);
  } else if (opponent_range.num_cards == NumCards::Two) {
    return compute_weights_3_vs_2(game, opponent_range);
  } else {
    return compute_weights_3_vs_3(game, opponent_range);
  }
}

static std::vector<uint32_t> sorted_hands_2_cards(const Game& game, const PokerHand& board) {
  std::vector<uint32_t> hands_sorted;
  hands_sorted.reserve(n_choose_k(MAX_DECK_SIZE - MAX_BOARD_CARDS, 2) + 2);

  // sentinels
  hands_sorted.push_back(0);
  hands_sorted.push_back(~0);

  const auto& hands = game.hands(NumCards::Two);

  for (unsigned i = 0; i < hands.size(); ++i) {
    const auto& cards = hands[i].cards;
    PokerHand hand{cards[0], cards[1]};

    if (!hand.collides_with(board)) {
      hand += board;
      const auto strength = hand.evaluate();
      hands_sorted.push_back(strength << 16 | i);
    }
  }

  hwy::VQSort(hands_sorted.data(), hands_sorted.size(), hwy::SortAscending{});
  return hands_sorted;
}

static std::vector<uint32_t> sorted_hands_3_cards(const Game& game, const PokerHand& board) {
  std::vector<uint32_t> hands_sorted;
  hands_sorted.reserve(n_choose_k(MAX_DECK_SIZE - MAX_BOARD_CARDS, 3) + 2);

  // sentinels
  hands_sorted.push_back(0);
  hands_sorted.push_back(~0);

  const auto& hands = game.hands(NumCards::Three);

  for (unsigned i = 0; i < hands.size(); ++i) {
    const auto& cards = hands[i].cards;
    PokerHand hand{cards[0], cards[1], cards[2]};

    if (!hand.collides_with(board)) {
      hand += board;
      const auto strength = hand.evaluate();
      hands_sorted.push_back(strength << 16 | i);
    }
  }

  hwy::VQSort(hands_sorted.data(), hands_sorted.size(), hwy::SortAscending{});
  return hands_sorted;
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_river_2_vs_3(const Game& game, const Range& opponent_range,
                                      const PokerHand& board, std::vector<T>& cfvs, double payoff,
                                      bool is_river_equity) {
  const auto hero_hands_sorted = sorted_hands_2_cards(game, board);
  const auto opponent_hands_sorted = sorted_hands_3_cards(game, board);

  const auto& hero_hands = game.hands(NumCards::Two);
  const auto& opponent_hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  unsigned j = 1;
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);

  // hero wins
  for (unsigned i = 1; i < hero_hands_sorted.size() - 1; ++i) {
    const auto& hero_hand = hero_hands_sorted[i];
    const int hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength <= opponent_strength) {
        break;
      }

      const int opponent_index = opponent_hand & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
      opponent_sum_1[opponent_cards[2]] += prob;

      ++j;
    }

    const auto& hero_cards = hero_hands[hero_index].cards;
    const double opponent_sum =
        opponent_sum_0 - opponent_sum_1[hero_cards[0]] - opponent_sum_1[hero_cards[1]];
    cfvs[hero_index] += opponent_sum * payoff;
  }

  j = opponent_hands_sorted.size() - 2;
  opponent_sum_0 = 0;
  std::fill(opponent_sum_1.begin(), opponent_sum_1.end(), 0);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  // hero loses
  for (unsigned i = hero_hands_sorted.size() - 2; i > 0; --i) {
    const auto& hero_hand = hero_hands_sorted[i];
    const int hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength >= opponent_strength) {
        break;
      }

      const int opponent_index = opponent_hand & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const auto& subs = subhand_indices[opponent_index];
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
      opponent_sum_1[opponent_cards[2]] += prob;
      opponent_sum_2[subs[0]] += prob;
      opponent_sum_2[subs[1]] += prob;
      opponent_sum_2[subs[2]] += prob;

      --j;
    }

    const auto& hero_cards = hero_hands[hero_index].cards;
    const double opponent_sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                                opponent_sum_1[hero_cards[1]] + opponent_sum_2[hero_index];
    cfvs[hero_index] -= opponent_sum * payoff;
  }

  if (is_river_equity) {
    for (; j > 0; --j) {
      const int opponent_index = opponent_hands_sorted[j] & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const auto& subs = subhand_indices[opponent_index];
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
      opponent_sum_1[opponent_cards[2]] += prob;
      opponent_sum_2[subs[0]] += prob;
      opponent_sum_2[subs[1]] += prob;
      opponent_sum_2[subs[2]] += prob;
    }

    for (unsigned i = 0; i < hero_hands.size(); ++i) {
      const auto& cards = hero_hands[i].cards;
      const T weight =
          opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] + opponent_sum_2[i];
      cfvs[i] = weight == 0.0 ? 0.0 : cfvs[i] / weight + 0.5;
    }
  }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_river_3_vs_2(const Game& game, const Range& opponent_range,
                                      const PokerHand& board, std::vector<T>& cfvs, double payoff,
                                      bool is_river_equity) {
  const auto hero_hands_sorted = sorted_hands_3_cards(game, board);
  const auto opponent_hands_sorted = sorted_hands_2_cards(game, board);

  const auto& hero_hands = game.hands(NumCards::Three);
  const auto& opponent_hands = game.hands(NumCards::Two);
  const auto& subhand_indices = game.subhand_indices();

  unsigned j = 1;
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  // hero wins
  for (unsigned i = 1; i < hero_hands_sorted.size() - 1; ++i) {
    const auto& hero_hand = hero_hands_sorted[i];
    const int hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength <= opponent_strength) {
        break;
      }

      const int opponent_index = opponent_hand & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
      opponent_sum_2[opponent_index] += prob;

      ++j;
    }

    const auto& hero_cards = hero_hands[hero_index].cards;
    const auto& subs = subhand_indices[hero_index];
    const double opponent_sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                                opponent_sum_1[hero_cards[1]] - opponent_sum_1[hero_cards[2]] +
                                opponent_sum_2[subs[0]] + opponent_sum_2[subs[1]] +
                                opponent_sum_2[subs[2]];
    cfvs[hero_index] += opponent_sum * payoff;
  }

  if (is_river_equity) {
    for (; j < opponent_hands_sorted.size() - 1; ++j) {
      const int opponent_index = opponent_hands_sorted[j] & ((1 << 16) - 1);
      const double prob = opponent_range.range[opponent_index];
      opponent_sum_2[opponent_index] += prob;
    }
  }

  j = opponent_hands_sorted.size() - 2;
  opponent_sum_0 = 0;
  std::fill(opponent_sum_1.begin(), opponent_sum_1.end(), 0);

  // hero loses
  for (unsigned i = hero_hands_sorted.size() - 2; i > 0; --i) {
    const auto& hero_hand = hero_hands_sorted[i];
    const int hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength >= opponent_strength) {
        break;
      }

      const int opponent_index = opponent_hand & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;

      --j;
    }

    const auto& hero_cards = hero_hands[hero_index].cards;
    const double opponent_sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                                opponent_sum_1[hero_cards[1]] - opponent_sum_1[hero_cards[2]];
    cfvs[hero_index] -= opponent_sum * payoff;
  }

  if (is_river_equity) {
    for (; j > 0; --j) {
      const int opponent_index = opponent_hands_sorted[j] & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
    }

    for (unsigned i = 0; i < hero_hands.size(); ++i) {
      const auto& cards = hero_hands[i].cards;
      const auto& subs = subhand_indices[i];
      const T weight = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] -
                       opponent_sum_1[cards[2]] + opponent_range.range[subs[0]] +
                       opponent_range.range[subs[1]] + opponent_range.range[subs[2]];
      cfvs[i] = weight == 0.0 ? 0.0 : cfvs[i] / weight + 0.5;
    }
  }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_river_3_vs_3(const Game& game, const Range& opponent_range,
                                      const PokerHand& board, std::vector<T>& cfvs, double payoff,
                                      bool is_river_equity) {
  const auto hero_hands_sorted = sorted_hands_3_cards(game, board);
  const auto opponent_hands_sorted = sorted_hands_3_cards(game, board);

  const auto& hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  unsigned j = 1;
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  // hero wins
  for (unsigned i = 1; i < hero_hands_sorted.size() - 1; ++i) {
    const auto& hero_hand = hero_hands_sorted[i];
    const int hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength <= opponent_strength) {
        break;
      }

      const int opponent_index = opponent_hand & ((1 << 16) - 1);
      const auto& opponent_cards = hands[opponent_index].cards;
      const auto& subs = subhand_indices[opponent_index];
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
      opponent_sum_1[opponent_cards[2]] += prob;
      opponent_sum_2[subs[0]] += prob;
      opponent_sum_2[subs[1]] += prob;
      opponent_sum_2[subs[2]] += prob;

      ++j;
    }

    const auto& hero_cards = hands[hero_index].cards;
    const auto& subs = subhand_indices[hero_index];
    const double opponent_sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                                opponent_sum_1[hero_cards[1]] - opponent_sum_1[hero_cards[2]] +
                                opponent_sum_2[subs[0]] + opponent_sum_2[subs[1]] +
                                opponent_sum_2[subs[2]];
    cfvs[hero_index] += opponent_sum * payoff;
  }

  j = opponent_hands_sorted.size() - 2;
  opponent_sum_0 = 0;
  std::fill(opponent_sum_1.begin(), opponent_sum_1.end(), 0);
  std::fill(opponent_sum_2.begin(), opponent_sum_2.end(), 0);

  // hero loses
  for (unsigned i = hero_hands_sorted.size() - 2; i > 0; --i) {
    const auto& hero_hand = hero_hands_sorted[i];
    const int hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength >= opponent_strength) {
        break;
      }

      const int opponent_index = opponent_hand & ((1 << 16) - 1);
      const auto& opponent_cards = hands[opponent_index].cards;
      const auto& subs = subhand_indices[opponent_index];
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
      opponent_sum_1[opponent_cards[2]] += prob;
      opponent_sum_2[subs[0]] += prob;
      opponent_sum_2[subs[1]] += prob;
      opponent_sum_2[subs[2]] += prob;

      --j;
    }

    const auto& hero_cards = hands[hero_index].cards;
    const auto& subs = subhand_indices[hero_index];
    const double opponent_sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                                opponent_sum_1[hero_cards[1]] - opponent_sum_1[hero_cards[2]] +
                                opponent_sum_2[subs[0]] + opponent_sum_2[subs[1]] +
                                opponent_sum_2[subs[2]];
    cfvs[hero_index] -= opponent_sum * payoff;
  }

  if (is_river_equity) {
    for (; j > 0; --j) {
      const int opponent_index = opponent_hands_sorted[j] & ((1 << 16) - 1);
      const auto& opponent_cards = hands[opponent_index].cards;
      const auto& subs = subhand_indices[opponent_index];
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
      opponent_sum_1[opponent_cards[2]] += prob;
      opponent_sum_2[subs[0]] += prob;
      opponent_sum_2[subs[1]] += prob;
      opponent_sum_2[subs[2]] += prob;
    }

    for (unsigned i = 0; i < hands.size(); ++i) {
      const auto& cards = hands[i].cards;
      const auto& subs = subhand_indices[i];
      const T weight = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] -
                       opponent_sum_1[cards[2]] + opponent_sum_2[subs[0]] +
                       opponent_sum_2[subs[1]] + opponent_sum_2[subs[2]] - opponent_range.range[i];
      cfvs[i] = weight == 0.0 ? 0.0 : cfvs[i] / weight + 0.5;
    }
  }
}

template <typename T, typename>
void compute_cfvs_river(const Game& game, const Range& hero_range, const Range& opponent_range,
                        const PokerHand& board, std::vector<T>& cfvs, double payoff,
                        bool is_river_equity) {
  if (hero_range.num_cards == NumCards::Two) {
    compute_cfvs_river_2_vs_3(game, opponent_range, board, cfvs, payoff, is_river_equity);
  } else if (opponent_range.num_cards == NumCards::Two) {
    compute_cfvs_river_3_vs_2(game, opponent_range, board, cfvs, payoff, is_river_equity);
  } else {
    compute_cfvs_river_3_vs_3(game, opponent_range, board, cfvs, payoff, is_river_equity);
  }
}

std::vector<float> compute_equities(const Game& game, const Range& hero_range,
                                    const Range& opponent_range,
                                    const std::vector<card_t>& board_cards) {
  if (board_cards.size() < 3) {
    throw std::invalid_argument("Board must have at least 3 cards");
  }
  if (hero_range.num_cards == NumCards::Two && opponent_range.num_cards == NumCards::Two) {
    throw std::invalid_argument("2 vs. 2 cards computations are not supported");
  }

  unsigned total_num_hole_cards = 5;
  if (hero_range.num_cards == NumCards::Three && opponent_range.num_cards == NumCards::Three) {
    total_num_hole_cards = 6;
  }

  PokerHand board(board_cards);

  // River
  if (board.size() == 5) {
    std::vector<float> equities(hero_range.num_hands());
    compute_cfvs_river(game, hero_range, opponent_range, board, equities, 0.5, true);
    return equities;
  }

  std::vector<double> cfvs(hero_range.num_hands());
  const unsigned num_remaining_cards = MAX_DECK_SIZE - total_num_hole_cards - board.size();

  if (board.size() == 3) {
    // Flop
    const double payoff = 1.0 / (num_remaining_cards * (num_remaining_cards - 1));
    for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
      if (!board.contains(i)) {
        for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
          if (!board.contains(j)) {
            const auto new_board = board + PokerHand{i, j};
            compute_cfvs_river(game, hero_range, opponent_range, new_board, cfvs, payoff);
          }
        }
      }
    }
  } else {
    // Turn
    const double payoff = 0.5 / num_remaining_cards;
    for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
      if (!board.contains(i)) {
        const auto new_board = board + PokerHand{i};
        compute_cfvs_river(game, hero_range, opponent_range, new_board, cfvs, payoff);
      }
    }
  }

  const auto weights = compute_weights(game, hero_range, opponent_range);

  std::vector<float> equities;
  equities.reserve(hero_range.num_hands());

  for (int i = 0; i < hero_range.num_hands(); ++i) {
    if (weights[i] > 0.0) {
      equities.push_back(cfvs[i] / weights[i] + 0.5);
    } else {
      equities.push_back(0.0);
    }
  }

  return equities;
}

}  // namespace pokerbot
