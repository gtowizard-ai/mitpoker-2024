#include <fmt/ranges.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>
#include "../src/equity.h"
#include "../src/game.h"
#include "../src/ranges_utils.h"

using namespace pokerbot;

class IsomorphicFlopEncoding {
 public:
  static unsigned constexpr NUM_FLOPS = 1755;

  // Custom/Ugly isomorphic flop encoder
  static std::string to_isomorphic_flop(const std::vector<card_t>& raw_cards) {
    assert(raw_cards.size() == 3);
    std::vector cards = {Card(raw_cards[0]), Card(raw_cards[1]), Card(raw_cards[2])};
    std::sort(cards.begin(), cards.end());
    const auto ranks = fmt::format("{}{}{}", cards[0].to_string()[0], cards[1].to_string()[0],
                                   cards[2].to_string()[0]);
    if (cards[0].suit() == cards[1].suit() && cards[1].suit() == cards[2].suit()) {
      // Monotone
      return fmt::format("{}m", ranks);
    } else if (cards[0].rank() == cards[1].rank() &&
               (cards[0].suit() == cards[2].suit() || cards[1].suit() == cards[2].suit())) {
      // Suited board on paired board
      return fmt::format("{}s", ranks);
    } else if (cards[1].rank() == cards[2].rank() &&
               (cards[0].suit() == cards[1].suit() || cards[0].suit() == cards[2].suit())) {
      // Suited board on paired board
      return fmt::format("{}s", ranks);
    } else if (cards[0].suit() == cards[1].suit()) {
      // Suited Low Middle
      return fmt::format("{}slm", ranks);
    } else if (cards[0].suit() == cards[2].suit()) {
      // Suited Low High
      return fmt::format("{}slh", ranks);
    } else if (cards[1].suit() == cards[2].suit()) {
      // Suited Middle High
      return fmt::format("{}smh", ranks);
    } else {
      // Rainbow
      return fmt::format("{}r", ranks);
    }
  }
};

void generate_equities() {
  std::unordered_set<std::string> seen_flops;
  std::ofstream output_file("flop_values.txt");
  Game game;
  const auto& two_cards_hands = game.hands(NumCards::Two);
  const auto& three_cards_hands = game.hands(NumCards::Three);
  int count = 0;

  if (!output_file.is_open()) {
    std::cout << "Failed to open output file" << std::endl;
  }

  for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
    for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
      for (card_t k = j + 1; k < MAX_DECK_SIZE; ++k) {
        std::vector<card_t> cards = {i, j, k};
        const auto board_str = IsomorphicFlopEncoding::to_isomorphic_flop(cards);

        if (seen_flops.find(board_str) != seen_flops.end()) {
          continue;
        }
        seen_flops.insert(board_str);

        Range two_card_range;
        Range three_card_range;
        two_card_range.update_on_board_cards(game, cards);
        three_card_range.update_on_board_cards(game, cards);
        three_card_range.to_3_cards_range(game, cards);

        std::vector<float> two_card_eq =
            compute_equities(game, two_card_range, three_card_range, cards);
        std::vector<float> three_card_eq =
            compute_equities(game, three_card_range, two_card_range, cards);

        const float normalization_factor = MAX_DECK_SIZE - cards.size() - 2;
        for (card_t card = 0; card < MAX_DECK_SIZE; ++card) {
          if (ranges::contains(cards, card)) {
            continue;
          }
          auto full_cards = cards;
          full_cards.push_back(card);

          for (hand_t h = 0; h < two_cards_hands.size(); ++h) {
            if (two_cards_hands[h].collides_with(full_cards)) {
              continue;
            }
            const auto three_card_hand =
                Hand(two_cards_hands[h].cards[0], two_cards_hands[h].cards[1], card);
            const auto h3_index = three_card_hand.index();
            two_card_eq[h] -= three_card_eq[h3_index] / normalization_factor;
          }
        }

        for (const std::string hand_str : {"2c2d", "Tc9c", "AhAs"}) {
          fmt::print("EQ difference for {} on {} is {:.4f} \n", hand_str, board_str,
                     two_card_eq[Hand(hand_str).index()]);
        }
        double avg_eq_diff =
            std::accumulate(two_card_eq.begin(), two_card_eq.end(), 0.0) / two_card_eq.size();
        fmt::print("{} - Average EQ difference = {} \n", board_str, avg_eq_diff);

        std::string output_string =
            "{" + fmt::format("\"{}\", {}", board_str, avg_eq_diff) + "},\n";
        output_file << output_string;
        count++;
        fmt::print("Ran over {} flops \n", count);
      }
    }
    fmt::print("{}/{} completed ({:.2f}%) \n", count, IsomorphicFlopEncoding::NUM_FLOPS,
               100 * (double)count / IsomorphicFlopEncoding::NUM_FLOPS);
  }
  output_file.close();
}

int main() {
  generate_equities();
  return 0;
}
