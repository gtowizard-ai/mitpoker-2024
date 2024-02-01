#include "poker_hand.h"

#include <gtest/gtest.h>

// #include <array>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

using namespace pokerbot;

// constexpr int num_categories = 9;

/// A compile-time for-each loop that calls `f(0)`, `f(1)`, ..., `f(N - 1)`.
template <int N, class F>
constexpr void constexpr_for_each(F&& f) {
  if constexpr (N > 0) {
    constexpr_for_each<N - 1>(std::forward<F>(f));
    f(std::integral_constant<int, N - 1>{});
  }
}

/// A compile-time enumeration of all hands of size `N`, calling `f` on each.
template <int N, class F, class... Args>
constexpr void enumerate_all_hands(F&& f, card_t card = 0, Args&&... args) {
  if constexpr (N == 0) {
    f(std::forward<Args>(args)...);
  } else {
    for (; card < MAX_DECK_SIZE; ++card) {
      enumerate_all_hands<N - 1>(std::forward<F>(f), card + 1, std::forward<Args>(args)..., card);
    }
  }
}

TEST(ToolsEvaluatorTest, BasicTests) {
  const PokerHand empty_hand;
  ASSERT_EQ(empty_hand.size(), 0);
  ASSERT_TRUE(empty_hand.empty());
  ASSERT_FALSE(empty_hand.contains(42));
  ASSERT_TRUE(empty_hand.contains(empty_hand));
  ASSERT_FALSE(empty_hand.collides_with(empty_hand));
  // ASSERT_EQ(empty_hand.evaluate(), 0);
  ASSERT_EQ(empty_hand, empty_hand);
  ASSERT_EQ(empty_hand, PokerHand(std::vector<card_t>{}));
  ASSERT_EQ(empty_hand, PokerHand(""));
  ASSERT_EQ(empty_hand + empty_hand, empty_hand);

  const PokerHand hand1{42};
  ASSERT_EQ(hand1.size(), 1);
  ASSERT_FALSE(hand1.empty());
  ASSERT_TRUE(hand1.contains(42));
  ASSERT_FALSE(hand1.contains(43));
  ASSERT_TRUE(hand1.contains(empty_hand));
  ASSERT_TRUE(hand1.contains(hand1));
  ASSERT_FALSE(empty_hand.contains(hand1));
  ASSERT_FALSE(hand1.collides_with(empty_hand));
  ASSERT_TRUE(hand1.collides_with(hand1));
  ASSERT_FALSE(empty_hand.collides_with(hand1));
  ASSERT_NE(empty_hand, hand1);
  ASSERT_EQ(hand1, hand1);
  ASSERT_EQ(empty_hand + hand1, hand1);
  ASSERT_EQ(hand1 + empty_hand, hand1);
  ASSERT_EQ(hand1 - empty_hand, hand1);
  ASSERT_EQ(hand1 - hand1, empty_hand);

  const PokerHand hand2{43};
  const PokerHand hand3{42, 43};
  ASSERT_EQ(hand3.size(), 2);
  ASSERT_FALSE(hand1.contains(hand2));
  ASSERT_FALSE(hand1.contains(hand3));
  ASSERT_TRUE(hand3.contains(hand1));
  ASSERT_FALSE(hand1.collides_with(hand2));
  ASSERT_TRUE(hand1.collides_with(hand3));
  ASSERT_TRUE(hand3.collides_with(hand1));
  ASSERT_EQ(hand1 + hand2, hand3);
  ASSERT_EQ(hand3 - hand1, hand2);
  ASSERT_EQ(hand3 - hand2, hand1);

  ASSERT_EQ(hand3, (PokerHand{43, 42}));
  ASSERT_EQ(hand3, PokerHand(std::vector<card_t>{42, 43}));
  ASSERT_EQ(hand3, PokerHand("QhQs"));
  ASSERT_EQ(hand3, PokerHand("QsQh"));

  // ASSERT_THROW(PokerHand{55}, std::out_of_range);
  // ASSERT_THROW((PokerHand{42, 42}), std::invalid_argument);
  // ASSERT_THROW(PokerHand(std::vector<card_t>{42, 55}), std::out_of_range);
  // ASSERT_THROW(PokerHand("A"), std::invalid_argument);
  // ASSERT_THROW(PokerHand("AA"), std::invalid_argument);
  // ASSERT_THROW(PokerHand("QhQsQh"), std::invalid_argument);
  // ASSERT_THROW(hand1.contains(55), std::out_of_range);
  // ASSERT_THROW(hand1 + hand3, std::invalid_argument);
  // ASSERT_THROW(hand1 - hand2, std::invalid_argument);

  const PokerHand hand4{2, 3, 5, 7, 11, 13, 17, 19};
  const PokerHand hand5 = hand4 + PokerHand{23};
  ASSERT_EQ(hand4.size(), 8);
  ASSERT_EQ(hand5.size(), 9);
  ASSERT_NO_THROW(hand4.evaluate());
  // ASSERT_THROW(hand5.evaluate(), std::runtime_error);
}

TEST(ToolsEvaluatorTest, AllCards) {
  PokerHand hand;

  for (card_t card = 0; card < MAX_DECK_SIZE; ++card) {
    ASSERT_FALSE(hand.contains(card));
    hand += PokerHand{card};
    ASSERT_TRUE(hand.contains(card));
  }

  // constructing a hand with all cards is valid
  ASSERT_EQ(hand.size(), MAX_DECK_SIZE);

  // evaluating a hand with all cards is not
  // ASSERT_THROW(hand.evaluate(), std::runtime_error);

  for (card_t card = 0; card < MAX_DECK_SIZE - 8; ++card) {
    ASSERT_TRUE(hand.contains(card));
    hand -= PokerHand{card};
    ASSERT_FALSE(hand.contains(card));
  }

  ASSERT_EQ(hand.size(), 8);
  ASSERT_EQ(hand, (PokerHand{44, 45, 46, 47, 48, 49, 50, 51}));
  // ASSERT_EQ(hand_category(hand.evaluate()), HandCategory::FourOfAKind);

  hand -= PokerHand{51};
  ASSERT_EQ(hand.size(), 7);
  ASSERT_EQ(hand, (PokerHand{44, 45, 46, 47, 48, 49, 50}));
  // ASSERT_EQ(hand_category(hand.evaluate()), HandCategory::FourOfAKind);

  hand -= PokerHand{44, 45, 46};
  ASSERT_EQ(hand.size(), 4);
  ASSERT_EQ(hand, (PokerHand{47, 48, 49, 50}));
  // ASSERT_EQ(hand_category(hand.evaluate()), HandCategory::ThreeOfAKind);

  hand -= hand;
  ASSERT_EQ(hand, PokerHand{});
}

/*

TEST(ToolsEvaluatorTest, WeakestHands) {
  const std::array hands = {"", "2c", "3c", "3c2c", "4c", "4c2c", "4c3c", "4c3c2c", "5c", "5c2c"};
  for (unsigned i = 0; i < hands.size(); ++i) {
    ASSERT_EQ(PokerHand(hands[i]).evaluate(), i);
  }
}

TEST(ToolsEvaluatorTest, IncompleteHands) {
  std::set<strength_t> strengths;

  constexpr_for_each<5>([&](auto N) {
    enumerate_all_hands<N>([&](auto&&... cards) {
      const auto hand = PokerHand{cards...};
      const auto strength = hand.evaluate();
      strengths.insert(strength);

      for (card_t card = 0; card < MAX_DECK_SIZE; ++card) {
        if (!hand.contains(card)) {
          const auto new_hand = hand + PokerHand{card};
          const auto new_strength = new_hand.evaluate();

          // check that adding a card always strictly increases the strength
          ASSERT_LT(strength, new_strength);
        }
      }
    });
  });

  // since there are no flushes, the strength is determined only by the ranks
  // the number of unique strengths should be equal to (14 multichoose 4) = (17 choose 4) = 2380
  ASSERT_EQ(strengths.size(), 2380);
}

*/

TEST(ToolsEvaluatorTest, EnumerateAll5CardHands) {
  std::set<strength_t> strengths;
  // std::array<int, num_categories> category_counts = {};

  enumerate_all_hands<5>([&](auto&&... cards) {
    const auto hand = PokerHand{cards...};
    const auto strength = hand.evaluate() & STRENGTH_MASK;
    strengths.insert(strength);
    // ++category_counts[static_cast<int>(hand_category(strength))];
  });

  // numbers from https://en.wikipedia.org/wiki/Poker_probability
  ASSERT_EQ(strengths.size(), 7462);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::HighCard)], 1302540);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Pair)], 1098240);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::TwoPair)], 123552);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::ThreeOfAKind)], 54912);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Straight)], 10200);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Flush)], 5108);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::FullHouse)], 3744);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::FourOfAKind)], 624);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::StraightFlush)], 40);
}

TEST(ToolsEvaluatorTest, EnumerateAll7CardHands) {
  std::set<strength_t> strengths;
  // std::array<int, num_categories> category_counts = {};

  enumerate_all_hands<5>([&](auto&&... cards) {
    const auto card5 = *std::rbegin({cards...});
    const auto hand5 = PokerHand{cards...};
    for (card_t card6 = card5 + 1; card6 < MAX_DECK_SIZE; ++card6) {
      const auto hand6 = hand5 + PokerHand{card6};
      for (card_t card7 = card6 + 1; card7 < MAX_DECK_SIZE; ++card7) {
        const auto hand = hand6 + PokerHand{card7};
        const auto strength = hand.evaluate() & STRENGTH_MASK;
        strengths.insert(strength);
        // ++category_counts[static_cast<int>(hand_category(strength))];
      }
    }
  });

  // numbers from https://en.wikipedia.org/wiki/Poker_probability
  ASSERT_EQ(strengths.size(), 4824);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::HighCard)], 23294460);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Pair)], 58627800);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::TwoPair)], 31433400);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::ThreeOfAKind)], 6461620);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Straight)], 6180020);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Flush)], 4047644);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::FullHouse)], 3473184);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::FourOfAKind)], 224848);
  // ASSERT_EQ(category_counts[static_cast<int>(HandCategory::StraightFlush)], 41584);
}

/*

TEST(ToolsEvaluatorTest, EnumerateAll8CardHands) {
  std::array<int, num_categories> category_counts = {};

  enumerate_all_hands<5>([&](auto&&... cards) {
    const auto card5 = *std::rbegin({cards...});
    const auto hand5 = PokerHand{cards...};
    for (card_t card6 = card5 + 1; card6 < MAX_DECK_SIZE; ++card6) {
      const auto hand6 = hand5 + PokerHand{card6};
      for (card_t card7 = card6 + 1; card7 < MAX_DECK_SIZE; ++card7) {
        const auto hand7 = hand6 + PokerHand{card7};
        for (card_t card8 = card7 + 1; card8 < MAX_DECK_SIZE; ++card8) {
          const auto hand = hand7 + PokerHand{card8};
          const auto strength = hand.evaluate();
          ++category_counts[static_cast<int>(hand_category(strength))];
        }
      }
    }
  });

  // numbers from:
  //   - https://wizardofodds.com/games/poker/
  //   - http://www.durangobill.com/Poker_Probabilities_8_Cards.html
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::HighCard)], 53476080);
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Pair)], 236092500);
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::TwoPair)], 257760900);
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::ThreeOfAKind)], 38493000);
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Straight)], 67072620);
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::Flush)], 50850320);
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::FullHouse)], 45652128);
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::FourOfAKind)], 2529262);
  ASSERT_EQ(category_counts[static_cast<int>(HandCategory::StraightFlush)], 611340);
}

*/
