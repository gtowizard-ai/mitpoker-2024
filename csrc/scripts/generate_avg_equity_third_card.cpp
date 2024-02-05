#include <fstream>
#include <iostream>
#include <vector>
#include "../src/equity.h"
#include "../src/game.h"
#include "../src/isomorphic_flop_encoder.h"
#include "../src/ranges_utils.h"

using namespace gtowizard;

void generate_equities(bool output_average_value = false) {
  std::ofstream output_file = [&] {
    if (output_average_value) {
      return std::ofstream("avg_equity_third_card.txt");
    } else {
      return std::ofstream("equity_third_card.bin", std::ios::out | std::ios::binary);
    }
  }();

  Game game;
  const auto& two_cards_hands = game.hands(NumCards::Two);
  int count = 0;

  if (!output_file.is_open()) {
    std::cout << "Failed to open output file" << std::endl;
  }

  for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
    for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
      for (card_t k = j + 1; k < MAX_DECK_SIZE; ++k) {
        std::vector<card_t> cards = {i, j, k};
        std::sort(cards.begin(), cards.end());

        const auto board_str = IsomorphicFlopEncoder::to_isomorphic_flop(cards);

        if (board_str != Card::to_string(cards)) {
          continue;
        }
        count++;

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
          fmt::print("EQ difference for {} on {}/{} is {:.4f} \n", hand_str, Card::to_string(cards),
                     board_str, two_card_eq[Hand(hand_str).index()]);
        }

        unsigned count_hand = 0;
        double avg_eq_diff = 0;
        for (hand_t h = 0; h < two_cards_hands.size(); ++h) {
          if (!two_cards_hands[h].collides_with(cards)) {
            count_hand++;
            avg_eq_diff += two_card_eq[h];
          }
        }
        avg_eq_diff /= static_cast<double>(count_hand);

        fmt::print("{} - Average EQ difference = {} \n", board_str, avg_eq_diff);

        if (output_average_value) {
          std::string output_string =
              "{" + fmt::format("\"{}\", {}", board_str, avg_eq_diff) + "},\n";
          output_file << output_string;
        } else {
          output_file.write(reinterpret_cast<const char*>(two_card_eq.data()),
                            sizeof(two_card_eq[0]) * two_card_eq.size());
        }

        fmt::print("{}/{} completed ({:.2f}%) \n", count, IsomorphicFlopEncoder::NUM_FLOPS,
                   100 * (double)count / IsomorphicFlopEncoder::NUM_FLOPS);
      }
    }
  }
  fmt::print("Total num of flops computed = {} \n", count);
  if (count != IsomorphicFlopEncoder::NUM_FLOPS) {
    throw std::runtime_error(fmt::format("Invalid num flops encountered = {}", count));
  }
  output_file.close();
}

int main() {
  generate_equities(false);
  return 0;
}
