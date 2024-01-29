#include "equity.h"
#include "poker_hand.h"

#include <numeric>
#include <stdexcept>

namespace pokerbot {

const std::vector<hand_t>& BoardDataCache::get_possible_hands_2_cards(
    const std::vector<card_t>& board_cards) {

  check_board(board_cards);
  if (possible_hands_2_cards_.has_value()) {
    return possible_hands_2_cards_.value();
  }

  possible_hands_2_cards_ = std::vector<hand_t>{};
  possible_hands_2_cards_->reserve(n_choose_k(MAX_DECK_SIZE - board_cards.size(), 2));

  const auto& hands = game_.hands(NumCards::Two);
  for (hand_t i = 0; i < hands.size(); ++i) {
    if (!hands[i].collides_with(board_cards)) {
      possible_hands_2_cards_->push_back(i);
    }
  }

  return possible_hands_2_cards_.value();
}

const std::vector<hand_t>& BoardDataCache::get_possible_hands_3_cards(
    const std::vector<card_t>& board_cards) {

  check_board(board_cards);
  if (possible_hands_3_cards_.has_value()) {
    return possible_hands_3_cards_.value();
  }

  possible_hands_3_cards_ = std::vector<hand_t>{};
  possible_hands_3_cards_->reserve(n_choose_k(MAX_DECK_SIZE - board_cards.size(), 3));

  const auto& hands = game_.hands(NumCards::Three);
  for (hand_t i = 0; i < hands.size(); ++i) {
    if (!hands[i].collides_with(board_cards)) {
      possible_hands_3_cards_->push_back(i);
    }
  }

  return possible_hands_3_cards_.value();
}

const std::vector<uint32_t>& BoardDataCache::get_sorted_hands_2_cards(
    const std::vector<card_t>& board_cards) {

  check_board(board_cards);
  if (sorted_hands_2_cards_.has_value()) {
    return sorted_hands_2_cards_.value();
  }

  sorted_hands_2_cards_ = std::vector<uint32_t>{};
  sorted_hands_2_cards_->reserve(n_choose_k(MAX_DECK_SIZE - MAX_BOARD_CARDS, 2) + 2);

  // sentinels
  sorted_hands_2_cards_->push_back(0);
  sorted_hands_2_cards_->push_back(~0);

  const PokerHand board(board_cards);
  const auto& hands = game_.hands(NumCards::Two);

  for (hand_t i = 0; i < hands.size(); ++i) {
    const auto& cards = hands[i].cards;
    PokerHand hand{cards[0], cards[1]};

    if (!hand.collides_with(board)) {
      hand += board;
      const auto strength = hand.evaluate();
      sorted_hands_2_cards_->push_back(strength << 16 | i);
    }
  }

  sort_hands(sorted_hands_2_cards_.value());
  return sorted_hands_2_cards_.value();
}

const std::vector<uint32_t>& BoardDataCache::get_sorted_hands_3_cards(
    const std::vector<card_t>& board_cards) {

  check_board(board_cards);
  if (sorted_hands_3_cards_.has_value()) {
    return sorted_hands_3_cards_.value();
  }

  sorted_hands_3_cards_ = std::vector<uint32_t>{};
  sorted_hands_3_cards_->reserve(n_choose_k(MAX_DECK_SIZE - MAX_BOARD_CARDS, 3) + 2);

  // sentinels
  sorted_hands_3_cards_->push_back(0);
  sorted_hands_3_cards_->push_back(~0);

  const PokerHand board(board_cards);
  const auto& hands = game_.hands(NumCards::Three);

  for (hand_t i = 0; i < hands.size(); ++i) {
    const auto& cards = hands[i].cards;
    PokerHand hand{cards[0], cards[1], cards[2]};

    if (!hand.collides_with(board)) {
      hand += board;
      const auto strength = hand.evaluate();
      sorted_hands_3_cards_->push_back(strength << 16 | i);
    }
  }

  sort_hands(sorted_hands_3_cards_.value());
  return sorted_hands_3_cards_.value();
}

void BoardDataCache::check_board(const std::vector<card_t>& board_cards) {
  if (board_cards == cached_board_) {
    return;
  }

  cached_board_ = board_cards;
  possible_hands_2_cards_.reset();
  possible_hands_3_cards_.reset();
  sorted_hands_2_cards_.reset();
  sorted_hands_3_cards_.reset();
}

void BoardDataCache::sort_hands(std::vector<uint32_t>& hands) {
  // TODO: replace with faster sorting library
  std::sort(hands.begin(), hands.end());
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_fixed_payoff_2_vs_2(const Game& game, const Range& opponent_range,
                                             const std::vector<card_t>& board_cards,
                                             std::vector<T>& cfvs, T payoff) {
  if (!board_cards.empty()) {
    throw std::invalid_argument("2 vs 2 assumes preflop");
  }

  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);

  const auto& hands = game.hands(NumCards::Two);
  for (hand_t i = 0; i < hands.size(); ++i) {
    const double prob = opponent_range.range[i];
    const auto& cards = hands[i].cards;
    opponent_sum_0 += prob;
    opponent_sum_1[cards[0]] += prob;
    opponent_sum_1[cards[1]] += prob;
  }

  for (hand_t i = 0; i < hands.size(); ++i) {
    const auto& cards = hands[i].cards;
    const T sum = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] +
                  opponent_range.range[i];
    cfvs[i] = sum * payoff;
  }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_fixed_payoff_2_vs_3(const Game& game, const Range& opponent_range,
                                             const std::vector<card_t>& board_cards,
                                             std::vector<T>& cfvs, T payoff,
                                             BoardDataCache& cache) {
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  const auto& opponent_hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  for (hand_t i = 0; i < opponent_hands.size(); ++i) {
    const double prob = opponent_range.range[i];
    const auto& cards = opponent_hands[i].cards;
    const auto& subs = subhand_indices[i];
    opponent_sum_0 += prob;
    opponent_sum_1[cards[0]] += prob;
    opponent_sum_1[cards[1]] += prob;
    opponent_sum_1[cards[2]] += prob;
    opponent_sum_2[subs[0]] += prob;
    opponent_sum_2[subs[1]] += prob;
    opponent_sum_2[subs[2]] += prob;
  }

  const auto& hero_hands = game.hands(NumCards::Two);
  const auto& possible_hands = cache.get_possible_hands_2_cards(board_cards);

  for (hand_t i : possible_hands) {
    const auto& cards = hero_hands[i].cards;
    const T sum =
        opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] + opponent_sum_2[i];
    cfvs[i] = sum * payoff;
  }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_fixed_payoff_3_vs_2(const Game& game, const Range& opponent_range,
                                             const std::vector<card_t>& board_cards,
                                             std::vector<T>& cfvs, T payoff,
                                             BoardDataCache& cache) {
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);

  const auto& opponent_hands = game.hands(NumCards::Two);

  for (hand_t i = 0; i < opponent_hands.size(); ++i) {
    const double prob = opponent_range.range[i];
    const auto& cards = opponent_hands[i].cards;
    opponent_sum_0 += prob;
    opponent_sum_1[cards[0]] += prob;
    opponent_sum_1[cards[1]] += prob;
  }

  const auto& hero_hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();
  const auto& possible_hands = cache.get_possible_hands_3_cards(board_cards);

  for (hand_t i : possible_hands) {
    const auto& cards = hero_hands[i].cards;
    const auto& subs = subhand_indices[i];
    const T sum = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] -
                  opponent_sum_1[cards[2]] + opponent_range.range[subs[0]] +
                  opponent_range.range[subs[1]] + opponent_range.range[subs[2]];
    cfvs[i] = sum * payoff;
  }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_fixed_payoff_3_vs_3(const Game& game, const Range& opponent_range,
                                             const std::vector<card_t>& board_cards,
                                             std::vector<T>& cfvs, T payoff,
                                             BoardDataCache& cache) {
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  const auto& hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  for (hand_t i = 0; i < hands.size(); ++i) {
    const double prob = opponent_range.range[i];
    const auto& cards = hands[i].cards;
    const auto& subs = subhand_indices[i];
    opponent_sum_0 += prob;
    opponent_sum_1[cards[0]] += prob;
    opponent_sum_1[cards[1]] += prob;
    opponent_sum_1[cards[2]] += prob;
    opponent_sum_2[subs[0]] += prob;
    opponent_sum_2[subs[1]] += prob;
    opponent_sum_2[subs[2]] += prob;
  }

  const auto& possible_hands = cache.get_possible_hands_3_cards(board_cards);

  for (hand_t i : possible_hands) {
    const auto& cards = hands[i].cards;
    const auto& subs = subhand_indices[i];
    const T sum = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] -
                  opponent_sum_1[cards[2]] + opponent_sum_2[subs[0]] + opponent_sum_2[subs[1]] +
                  opponent_sum_2[subs[2]] - opponent_range.range[i];
    cfvs[i] = sum * payoff;
  }
}

// Assume that probs of hands that are blocked by board cards are properly set to zero
template <typename T, typename>
void compute_cfvs_fixed_payoff(const Game& game, const Range& hero_range,
                               const Range& opponent_range, const std::vector<card_t>& board_cards,
                               std::vector<T>& cfvs, T payoff, BoardDataCache& cache) {
  if (hero_range.num_cards == NumCards::Two && opponent_range.num_cards == NumCards::Two) {
    compute_cfvs_fixed_payoff_2_vs_2(game, opponent_range, board_cards, cfvs, payoff);
  } else if (hero_range.num_cards == NumCards::Two) {
    compute_cfvs_fixed_payoff_2_vs_3(game, opponent_range, board_cards, cfvs, payoff, cache);
  } else if (opponent_range.num_cards == NumCards::Two) {
    compute_cfvs_fixed_payoff_3_vs_2(game, opponent_range, board_cards, cfvs, payoff, cache);
  } else {
    compute_cfvs_fixed_payoff_3_vs_3(game, opponent_range, board_cards, cfvs, payoff, cache);
  }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_showdown_2_vs_3(const Game& game, const Range& opponent_range,
                                         const std::vector<card_t>& board_cards,
                                         std::vector<T>& cfvs, T payoff, BoardDataCache& cache,
                                         bool is_river_equity) {
  const auto& hero_hands_sorted = cache.get_sorted_hands_2_cards(board_cards);
  const auto& opponent_hands_sorted = cache.get_sorted_hands_3_cards(board_cards);

  const auto& hero_hands = game.hands(NumCards::Two);
  const auto& opponent_hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  unsigned j = 1;
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);

  // hero wins
  for (unsigned i = 1; i < hero_hands_sorted.size() - 1; ++i) {
    const auto& hero_hand = hero_hands_sorted[i];
    const hand_t hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength <= opponent_strength) {
        break;
      }

      const hand_t opponent_index = opponent_hand & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
      opponent_sum_1[opponent_cards[2]] += prob;

      ++j;
    }

    const auto& hero_cards = hero_hands[hero_index].cards;
    const double sum =
        opponent_sum_0 - opponent_sum_1[hero_cards[0]] - opponent_sum_1[hero_cards[1]];
    cfvs[hero_index] += sum * payoff;
  }

  j = opponent_hands_sorted.size() - 2;
  opponent_sum_0 = 0;
  std::fill(opponent_sum_1.begin(), opponent_sum_1.end(), 0);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  // hero loses
  for (unsigned i = hero_hands_sorted.size() - 2; i > 0; --i) {
    const auto& hero_hand = hero_hands_sorted[i];
    const hand_t hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength >= opponent_strength) {
        break;
      }

      const hand_t opponent_index = opponent_hand & ((1 << 16) - 1);
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
    const double sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                       opponent_sum_1[hero_cards[1]] + opponent_sum_2[hero_index];
    cfvs[hero_index] -= sum * payoff;
  }

  if (is_river_equity) {
    for (; j > 0; --j) {
      const hand_t opponent_index = opponent_hands_sorted[j] & ((1 << 16) - 1);
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

    for (hand_t i = 0; i < hero_hands.size(); ++i) {
      const auto& cards = hero_hands[i].cards;
      const T sum =
          opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] + opponent_sum_2[i];
      cfvs[i] = sum == 0.0 ? 0.0 : cfvs[i] / sum + 0.5;
    }
  }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_showdown_3_vs_2(const Game& game, const Range& opponent_range,
                                         const std::vector<card_t>& board_cards,
                                         std::vector<T>& cfvs, T payoff, BoardDataCache& cache,
                                         bool is_river_equity) {
  const auto& hero_hands_sorted = cache.get_sorted_hands_3_cards(board_cards);
  const auto& opponent_hands_sorted = cache.get_sorted_hands_2_cards(board_cards);

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
    const hand_t hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength <= opponent_strength) {
        break;
      }

      const hand_t opponent_index = opponent_hand & ((1 << 16) - 1);
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
    const double sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                       opponent_sum_1[hero_cards[1]] - opponent_sum_1[hero_cards[2]] +
                       opponent_sum_2[subs[0]] + opponent_sum_2[subs[1]] + opponent_sum_2[subs[2]];
    cfvs[hero_index] += sum * payoff;
  }

  if (is_river_equity) {
    for (; j < opponent_hands_sorted.size() - 1; ++j) {
      const hand_t opponent_index = opponent_hands_sorted[j] & ((1 << 16) - 1);
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
    const hand_t hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = opponent_hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength >= opponent_strength) {
        break;
      }

      const hand_t opponent_index = opponent_hand & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;

      --j;
    }

    const auto& hero_cards = hero_hands[hero_index].cards;
    const double sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                       opponent_sum_1[hero_cards[1]] - opponent_sum_1[hero_cards[2]];
    cfvs[hero_index] -= sum * payoff;
  }

  if (is_river_equity) {
    for (; j > 0; --j) {
      const hand_t opponent_index = opponent_hands_sorted[j] & ((1 << 16) - 1);
      const auto& opponent_cards = opponent_hands[opponent_index].cards;
      const double prob = opponent_range.range[opponent_index];

      opponent_sum_0 += prob;
      opponent_sum_1[opponent_cards[0]] += prob;
      opponent_sum_1[opponent_cards[1]] += prob;
    }

    for (hand_t i = 0; i < hero_hands.size(); ++i) {
      const auto& cards = hero_hands[i].cards;
      const auto& subs = subhand_indices[i];
      const T sum = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] -
                    opponent_sum_1[cards[2]] + opponent_range.range[subs[0]] +
                    opponent_range.range[subs[1]] + opponent_range.range[subs[2]];
      cfvs[i] = sum == 0.0 ? 0.0 : cfvs[i] / sum + 0.5;
    }
  }
}

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
static void compute_cfvs_showdown_3_vs_3(const Game& game, const Range& opponent_range,
                                         const std::vector<card_t>& board_cards,
                                         std::vector<T>& cfvs, T payoff, BoardDataCache& cache,
                                         bool is_river_equity) {
  const auto& hands_sorted = cache.get_sorted_hands_3_cards(board_cards);

  const auto& hands = game.hands(NumCards::Three);
  const auto& subhand_indices = game.subhand_indices();

  unsigned j = 1;
  double opponent_sum_0 = 0;
  std::vector<double> opponent_sum_1(MAX_DECK_SIZE);
  std::vector<double> opponent_sum_2(NUM_HANDS_POSTFLOP_2CARDS);

  // hero wins
  for (unsigned i = 1; i < hands_sorted.size() - 1; ++i) {
    const auto& hero_hand = hands_sorted[i];
    const hand_t hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength <= opponent_strength) {
        break;
      }

      const hand_t opponent_index = opponent_hand & ((1 << 16) - 1);
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
    const double sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                       opponent_sum_1[hero_cards[1]] - opponent_sum_1[hero_cards[2]] +
                       opponent_sum_2[subs[0]] + opponent_sum_2[subs[1]] + opponent_sum_2[subs[2]];
    cfvs[hero_index] += sum * payoff;
  }

  j = hands_sorted.size() - 2;
  opponent_sum_0 = 0;
  std::fill(opponent_sum_1.begin(), opponent_sum_1.end(), 0);
  std::fill(opponent_sum_2.begin(), opponent_sum_2.end(), 0);

  // hero loses
  for (unsigned i = hands_sorted.size() - 2; i > 0; --i) {
    const auto& hero_hand = hands_sorted[i];
    const hand_t hero_index = hero_hand & ((1 << 16) - 1);
    const int hero_strength = hero_hand >> 16;

    while (true) {
      const auto& opponent_hand = hands_sorted[j];
      const int opponent_strength = opponent_hand >> 16;

      if (hero_strength >= opponent_strength) {
        break;
      }

      const hand_t opponent_index = opponent_hand & ((1 << 16) - 1);
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
    const double sum = opponent_sum_0 - opponent_sum_1[hero_cards[0]] -
                       opponent_sum_1[hero_cards[1]] - opponent_sum_1[hero_cards[2]] +
                       opponent_sum_2[subs[0]] + opponent_sum_2[subs[1]] + opponent_sum_2[subs[2]];
    cfvs[hero_index] -= sum * payoff;
  }

  if (is_river_equity) {
    for (; j > 0; --j) {
      const hand_t opponent_index = hands_sorted[j] & ((1 << 16) - 1);
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

    for (hand_t i = 0; i < hands.size(); ++i) {
      const auto& cards = hands[i].cards;
      const auto& subs = subhand_indices[i];
      const T sum = opponent_sum_0 - opponent_sum_1[cards[0]] - opponent_sum_1[cards[1]] -
                    opponent_sum_1[cards[2]] + opponent_sum_2[subs[0]] + opponent_sum_2[subs[1]] +
                    opponent_sum_2[subs[2]] - opponent_range.range[i];
      cfvs[i] = sum == 0.0 ? 0.0 : cfvs[i] / sum + 0.5;
    }
  }
}

template <typename T, typename>
void compute_cfvs_showdown(const Game& game, const Range& hero_range, const Range& opponent_range,
                           const std::vector<card_t>& board_cards, std::vector<T>& cfvs, T payoff,
                           BoardDataCache& cache, bool is_river_equity) {
  if (hero_range.num_cards == NumCards::Two && opponent_range.num_cards == NumCards::Two) {
    throw std::invalid_argument("CFV - 2 vs. 2 cards computations are not supported");
  }

  if (hero_range.num_cards == NumCards::Two) {
    compute_cfvs_showdown_2_vs_3(game, opponent_range, board_cards, cfvs, payoff, cache,
                                 is_river_equity);
  } else if (opponent_range.num_cards == NumCards::Two) {
    compute_cfvs_showdown_3_vs_2(game, opponent_range, board_cards, cfvs, payoff, cache,
                                 is_river_equity);
  } else {
    compute_cfvs_showdown_3_vs_3(game, opponent_range, board_cards, cfvs, payoff, cache,
                                 is_river_equity);
  }
}

std::vector<float> compute_equities(const Game& game, const Range& hero_range,
                                    const Range& opponent_range,
                                    const std::vector<card_t>& board_cards) {
  if (board_cards.size() < 3) {
    throw std::invalid_argument("Board must have at least 3 cards");
  }
  if (hero_range.num_cards == NumCards::Two && opponent_range.num_cards == NumCards::Two) {
    throw std::invalid_argument("Equities - 2 vs. 2 cards computations are not supported");
  }

  BoardDataCache cache(game);

  // River
  if (board_cards.size() == 5) {
    std::vector<float> equities(hero_range.num_hands());
    compute_cfvs_showdown(game, hero_range, opponent_range, board_cards, equities, 0.5f, cache,
                          true);
    return equities;
  }

  unsigned total_num_hole_cards = 5;
  if (hero_range.num_cards == NumCards::Three && opponent_range.num_cards == NumCards::Three) {
    total_num_hole_cards = 6;
  }

  const unsigned num_remaining_cards = MAX_DECK_SIZE - total_num_hole_cards - board_cards.size();
  const uint64_t board_bitset =
      std::accumulate(board_cards.cbegin(), board_cards.cend(), 0ull,
                      [](uint64_t acc, card_t card) { return acc | (1ull << card); });

  std::vector<card_t> new_board = board_cards;
  new_board.resize(5);

  std::vector<double> cfvs(hero_range.num_hands());

  if (board_cards.size() == 3) {
    // Flop
    const double payoff = 1.0 / (num_remaining_cards * (num_remaining_cards - 1));
    for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
      if ((board_bitset & (1ull << i)) == 0) {
        for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
          if ((board_bitset & (1ull << j)) == 0) {
            new_board[3] = i;
            new_board[4] = j;
            compute_cfvs_showdown(game, hero_range, opponent_range, new_board, cfvs, payoff, cache);
          }
        }
      }
    }
  } else {
    // Turn
    const double payoff = 0.5 / num_remaining_cards;
    for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
      if ((board_bitset & (1ull << i)) == 0) {
        new_board[4] = i;
        compute_cfvs_showdown(game, hero_range, opponent_range, new_board, cfvs, payoff, cache);
      }
    }
  }

  std::vector<double> sums(hero_range.num_hands());
  compute_cfvs_fixed_payoff(game, hero_range, opponent_range, board_cards, sums, 1.0, cache);

  std::vector<float> equities;
  equities.reserve(hero_range.num_hands());

  for (hand_t i = 0; i < hero_range.num_hands(); ++i) {
    if (sums[i] > 0.0) {
      equities.push_back(cfvs[i] / sums[i] + 0.5);
    } else {
      equities.push_back(0.0);
    }
  }

  return equities;
}

// Explicit template initializations

template void compute_cfvs_fixed_payoff<float>(const Game& game, const Range& hero_range,
                                               const Range& opponent_range,
                                               const std::vector<card_t>& board_cards,
                                               std::vector<float>& cfvs, float payoff,
                                               BoardDataCache& cache);
template void compute_cfvs_fixed_payoff<double>(const Game& game, const Range& hero_range,
                                                const Range& opponent_range,
                                                const std::vector<card_t>& board_cards,
                                                std::vector<double>& cfvs, double payoff,
                                                BoardDataCache& cache);

template void compute_cfvs_showdown<float>(const Game& game, const Range& hero_range,
                                           const Range& opponent_range,
                                           const std::vector<card_t>& board_cards,
                                           std::vector<float>& cfvs, float payoff,
                                           BoardDataCache& cache, bool is_river_equity);
template void compute_cfvs_showdown<double>(const Game& game, const Range& hero_range,
                                            const Range& opponent_range,
                                            const std::vector<card_t>& board_cards,
                                            std::vector<double>& cfvs, double payoff,
                                            BoardDataCache& cache, bool is_river_equity);

}  // namespace pokerbot
