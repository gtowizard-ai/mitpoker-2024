#include "equity.h"
#include <gtest/gtest.h>
#include "game.h"
#include "poker_hand.h"
#include "range.h"
#include "ranges_utils.h"
#include "round.h"

using namespace pokerbot;

class EquityTest : public ::testing::Test {
 protected:
  Game game_;

  static PokerHand to_poker_hand(const Hand& hand) {
    // TODO - Not very efficient
    return PokerHand({hand.cards.begin(), hand.cards.begin() + hand.num_cards()});
  }

  static void assert_equities_values(const std::vector<float>& equities) {
    ASSERT_TRUE(std::all_of(equities.begin(), equities.end(), [](auto equity) {
      return std::isfinite(equity) && equity >= 0 && equity <= 1.0;
    }));
  }

  Range get_uniform_random_range(const std::vector<card_t>& board_cards) const {
    Range range;
    range.update_on_board_cards(game_, board_cards);
    return range;
  }

  double naive_compute_equity(const Hand& hero_hand, const Range& opponent_range,
                              const std::vector<card_t>& board_cards) const {
    const auto num_cards_left = MAX_BOARD_CARDS - board_cards.size();
    std::vector<card_t> cards_left;
    cards_left.reserve(num_cards_left);
    for (card_t card = 0; card < MAX_DECK_SIZE; ++card) {
      if (!ranges::contains(board_cards, card)) {
        cards_left.push_back(card);
      }
    }

    Results results;

    if (num_cards_left == 0) {
      naive_eval_one_river(hero_hand, opponent_range, board_cards, results);
    } else if (num_cards_left == 1) {
      for (auto card : cards_left) {
        auto river_cards = board_cards;
        river_cards.push_back(card);
        naive_eval_one_river(hero_hand, opponent_range, river_cards, results);
      }
    } else if (num_cards_left == 2) {
      for (unsigned i = 0; i < cards_left.size(); ++i) {
        for (unsigned j = i + 1; j < cards_left.size(); ++j) {
          auto river_cards = board_cards;
          river_cards.push_back(cards_left[i]);
          river_cards.push_back(cards_left[j]);
          naive_eval_one_river(hero_hand, opponent_range, river_cards, results);
        }
      }
    } else {
      throw std::runtime_error("Preflop not implemented");
    }

    if (results.count == 0) {
      return 0;
    }
    double equity = (results.wins + 0.5 * results.ties) / results.count;
    return equity;
  }

 private:
  struct Results {
    double wins = 0;
    double ties = 0;
    double count = 0;
  };

  void naive_eval_one_river(const Hand& hand, const Range& opponent_range,
                            const std::vector<card_t>& board_cards, Results& results) const {
    ASSERT_EQ(board_cards.size(), round::RIVER.num_board_cards);
    if (hand.collides_with(board_cards)) {
      return;
    }

    const auto& opponent_hands = game_.hands(opponent_range.num_cards);
    const auto eval_board = PokerHand(board_cards);

    auto hero_hand = eval_board + to_poker_hand(hand);
    for (hand_t i = 0; i < opponent_range.num_hands(); ++i) {
      if (hand.collides_with(opponent_hands[i]) || opponent_hands[i].collides_with(board_cards)) {
        continue;
      }
      auto opponent_hand = eval_board + to_poker_hand(opponent_hands[i]);
      auto hero_strength = hero_hand.evaluate();
      auto opponent_strength = opponent_hand.evaluate();
      if (hero_strength > opponent_strength) {
        results.wins += opponent_range.range[i];
      } else if (hero_strength == opponent_strength) {
        results.ties += opponent_range.range[i];
      }
      results.count += opponent_range.range[i];
    }
  }
};

TEST_F(EquityTest, TestNaiveEquityComputationRiverTwoVsTwoCards) {
  const auto board_cards = Card::to_vector("AcKdTh2s8s");
  const auto opponent_range = get_uniform_random_range(board_cards);

  ASSERT_EQ(naive_compute_equity(Hand("2c2s"), opponent_range, board_cards), 0);
  ASSERT_EQ(naive_compute_equity(Hand("As2s"), opponent_range, board_cards), 0);
  ASSERT_NEAR(naive_compute_equity(Hand("2c2d"), opponent_range, board_cards), 0.9717, 1e-4);
  ASSERT_NEAR(naive_compute_equity(Hand("QhJh"), opponent_range, board_cards), 0.9955, 1e-4);
  ASSERT_NEAR(naive_compute_equity(Hand("Tc7h"), opponent_range, board_cards), 0.6485, 1e-4);
}

TEST_F(EquityTest, TestNaiveEquityComputationTurnTwoVsTwoCards) {
  const auto board_cards = Card::to_vector("AcKc6h2s");
  const auto opponent_range = get_uniform_random_range(board_cards);

  ASSERT_NEAR(naive_compute_equity(Hand("QcJc"), opponent_range, board_cards), 0.6095, 1e-4);
  ASSERT_NEAR(naive_compute_equity(Hand("6s2d"), opponent_range, board_cards), 0.8855, 1e-4);
  ASSERT_NEAR(naive_compute_equity(Hand("As8h"), opponent_range, board_cards), 0.8656, 1e-4);
  ASSERT_NEAR(naive_compute_equity(Hand("As8c"), opponent_range, board_cards), 0.8686, 1e-4);
}

TEST_F(EquityTest, TestNaiveEquityComputationFlopTwoVsTwoCards) {
  const auto board_cards = Card::to_vector("AcKc6h");
  Range hero_range;
  Range opponent_range;

  hero_range.update_on_board_cards(game_, board_cards);
  opponent_range.update_on_board_cards(game_, board_cards);

  ASSERT_NEAR(naive_compute_equity(Hand("QcJc"), opponent_range, board_cards), 0.7319, 1e-4);
  ASSERT_NEAR(naive_compute_equity(Hand("6s2d"), opponent_range, board_cards), 0.5843, 1e-4);
  ASSERT_NEAR(naive_compute_equity(Hand("As8h"), opponent_range, board_cards), 0.8364, 1e-4);
  ASSERT_NEAR(naive_compute_equity(Hand("As8c"), opponent_range, board_cards), 0.8569, 1e-4);
}

TEST_F(EquityTest, TestNaiveEquityComputationRiverThreeVsTwoCards) {
  const auto board_cards = Card::to_vector("AcKdTh2s8s");
  const auto opponent_range = get_uniform_random_range(board_cards);

  ASSERT_EQ(naive_compute_equity(Hand("2c2s2h"), opponent_range, board_cards), 0);
  ASSERT_EQ(naive_compute_equity(Hand("AcKs8h"), opponent_range, board_cards), 0);
  // Can only split with QJ -- 2 Q left * 2 J left in the deck
  auto tie_prob = 2 * (2.0 / 44.0 * 3.0 / 43.0);
  ASSERT_NEAR(naive_compute_equity(Hand("QcQhJc"), opponent_range, board_cards),
              1.0 - 0.5 * tie_prob, 1e-6);
}

TEST_F(EquityTest, TestNaiveEquityComputationRiverTwoVsThreeCards) {
  const auto board_cards = Card::to_vector("8c8d6h4s2s");
  auto opponent_range = get_uniform_random_range(board_cards);
  opponent_range.to_3_cards_range(game_, board_cards);

  ASSERT_EQ(naive_compute_equity(Hand("2c2s"), opponent_range, board_cards), 0);
  ASSERT_EQ(naive_compute_equity(Hand("8d8s"), opponent_range, board_cards), 0.0);
  // We lose to exactly {3s, 5s, 6s} and {As, 3s, 5s}
  ASSERT_NEAR(naive_compute_equity(Hand("8h8s"), opponent_range, board_cards),
              1.0 - (2.0 / n_choose_k(45, 3)), 1e-6);
}

TEST_F(EquityTest, TestNaiveEquityComputationRiverThreeVsThreeCards) {
  const auto board_cards = Card::to_vector("8c8d6h4s2s");
  auto opponent_range = get_uniform_random_range(board_cards);
  opponent_range.to_3_cards_range(game_, board_cards);

  ASSERT_EQ(naive_compute_equity(Hand("3s5s6s"), opponent_range, board_cards), 1.0);
  // We lose to exactly {3s, 5s, 6s} and {As, 3s, 5s}
  ASSERT_NEAR(naive_compute_equity(Hand("8h8s"), opponent_range, board_cards),
              1.0 - (2.0 / n_choose_k(45, 3)), 1e-6);
}

TEST_F(EquityTest, TestEquityComputationRiverThreeVsTwoCards) {
  const auto board_cards = Card::to_vector("8c8d6h4s2s");
  const auto opponent_range = get_uniform_random_range(board_cards);
  auto hero_range = get_uniform_random_range(board_cards);
  hero_range.to_3_cards_range(game_, board_cards);

  const auto equities = compute_equities(game_, hero_range, opponent_range, board_cards);
  ASSERT_EQ(equities.size(), NUM_HANDS_POSTFLOP_3CARDS);
  assert_equities_values(equities);

  for (std::string hand_str : {"3s5s6s", "7h8sAc", "AcAdAh", "8s8hAs", "QcJdTh"}) {
    Hand hand(hand_str);
    EXPECT_NEAR(equities[hand.index()], naive_compute_equity(hand, opponent_range, board_cards),
                1e-4)
        << hand_str;
  }
}

TEST_F(EquityTest, TestEquityComputationRiverTwoVsThreeCards) {
  const auto board_cards = Card::to_vector("8c8d6h4s2s");
  const auto hero_range = get_uniform_random_range(board_cards);
  auto opponent_range = get_uniform_random_range(board_cards);
  opponent_range.to_3_cards_range(game_, board_cards);

  const auto equities = compute_equities(game_, hero_range, opponent_range, board_cards);
  ASSERT_EQ(equities.size(), NUM_HANDS_POSTFLOP_2CARDS);
  assert_equities_values(equities);

  for (std::string hand_str : {"3s5s", "7h8s", "AcAd", "8s8h", "QcJd"}) {
    Hand hand(hand_str);
    EXPECT_NEAR(equities[hand.index()], naive_compute_equity(hand, opponent_range, board_cards),
                1e-4)
        << hand_str;
  }
}

TEST_F(EquityTest, TestEquityComputationRiverThreeVsThreeCards) {
  const auto board_cards = Card::to_vector("8c8d6h4s2s");
  auto hero_range = get_uniform_random_range(board_cards);
  auto opponent_range = get_uniform_random_range(board_cards);
  hero_range.to_3_cards_range(game_, board_cards);
  opponent_range.to_3_cards_range(game_, board_cards);

  const auto equities = compute_equities(game_, hero_range, opponent_range, board_cards);
  ASSERT_EQ(equities.size(), NUM_HANDS_POSTFLOP_3CARDS);
  assert_equities_values(equities);

  for (std::string hand_str : {"3s5s6s", "7h8sAc", "AcAdAh", "8s8h6c", "QcJdTh"}) {
    Hand hand(hand_str);
    EXPECT_NEAR(equities[hand.index()], naive_compute_equity(hand, opponent_range, board_cards),
                1e-4)
        << hand_str;
  }
}

TEST_F(EquityTest, TestEquityComputationTurnTwoVsThreeCards) {
  const auto board_cards = Card::to_vector("8c8d6h4s");
  const auto hero_range = get_uniform_random_range(board_cards);
  auto opponent_range = get_uniform_random_range(board_cards);
  opponent_range.to_3_cards_range(game_, board_cards);

  const auto equities = compute_equities(game_, hero_range, opponent_range, board_cards);
  ASSERT_EQ(equities.size(), NUM_HANDS_POSTFLOP_2CARDS);
  assert_equities_values(equities);

  for (std::string hand_str : {"3s5s", "7h8s", "AcAd", "8s8h", "QcJd"}) {
    Hand hand(hand_str);
    EXPECT_NEAR(equities[hand.index()], naive_compute_equity(hand, opponent_range, board_cards),
                1e-4)
        << hand_str;
  }
}

TEST_F(EquityTest, TestEquityComputationTurnThreeVsThreeCards) {
  const auto board_cards = Card::to_vector("8c8d6h4s");
  auto hero_range = get_uniform_random_range(board_cards);
  auto opponent_range = get_uniform_random_range(board_cards);
  hero_range.to_3_cards_range(game_, board_cards);
  opponent_range.to_3_cards_range(game_, board_cards);

  const auto equities = compute_equities(game_, hero_range, opponent_range, board_cards);
  ASSERT_EQ(equities.size(), NUM_HANDS_POSTFLOP_3CARDS);
  assert_equities_values(equities);

  for (std::string hand_str : {"3s5s6s", "7h8sAd", "AcAdKc", "8s8h2c", "AcQcJd"}) {
    Hand hand(hand_str);
    EXPECT_NEAR(equities[hand.index()], naive_compute_equity(hand, opponent_range, board_cards),
                1e-4)
        << hand_str;
  }
}

TEST_F(EquityTest, TestComputeCFVsFoldNode) {
  const auto board_cards = Card::to_vector("8c8d6h4s2s");
  auto hero_range = get_uniform_random_range(board_cards);
  auto opponent_range = get_uniform_random_range(board_cards);
  hero_range.to_3_cards_range(game_, board_cards);
  opponent_range.to_3_cards_range(game_, board_cards);

  std::vector<double> cfvs(NUM_HANDS_POSTFLOP_3CARDS);
  PokerHand board(board_cards);
  compute_cfvs_fixed_payoff(game_, hero_range, opponent_range, cfvs, -1.0);

  const auto& hands = game_.hands(hero_range.num_cards);
  for (unsigned i = 0; i < cfvs.size(); ++i) {
    if (hands[i].collides_with(board_cards)) {
      ASSERT_EQ(cfvs[i], 0);
    } else {
      ASSERT_EQ(cfvs[i], cfvs[0]);
    }
  }
}
