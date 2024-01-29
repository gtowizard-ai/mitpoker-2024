#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "auction.h"

namespace pokerbot {

void generate_flop_bids() {
  const std::vector<char> ranks = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
  const std::vector<char> suits = {'s', 'h', 'd', 'c'};
  std::map<std::string, bool> seen_flops;
  std::ofstream output_file("~/Desktop/mitpokerbots-2024/flop_values.txt");
  Game game_;
  Range r1;
  Auctioneer auctioneer;
  int count = 0;

  if (!output_file.is_open()) {
    std::cout << "Failed to open output file" << std::endl;
  }

  for (std::size_t i = 0; i < ranks.size(); ++i) {
    for (std::size_t j = 0; j < suits.size(); ++j) {
      for (std::size_t k = i; k < ranks.size(); ++k) {
        for (std::size_t l = 0; l < suits.size(); ++l) {
          for (std::size_t m = k; m < ranks.size(); ++m) {
            for (std::size_t n = 0; n < suits.size(); ++n) {
              if (!(i == k && j == l) && !(i == m && j == n) && !(m == k && n == l)) {
                std::string card_1 = std::string(1, ranks[i]) + suits[j];
                std::string card_2 = std::string(1, ranks[k]) + suits[l];
                std::string card_3 = std::string(1, ranks[m]) + suits[n];
                std::vector<std::string> cards = {card_1, card_2, card_3};
                std::sort(cards.begin(), cards.end());
                std::string flop;
                for (const std::string& c : cards) {
                  flop += c;
                }

                if (seen_flops.find(flop) != seen_flops.end()) {
                  continue;
                };

                if (count == 10) {
                  return;
                }

                auto flop_as_cards = Card::to_vector(flop);
                Range r2;
                r2.to_3_cards_range(game_, flop_as_cards);
                float equity = auctioneer.mean_equity(r2, r1, game_, flop_as_cards) -
                               auctioneer.mean_equity(r1, r2, game_, flop_as_cards);
                std::string output_string = "{";
                output_string += flop;
                output_string += ", ";
                output_string += std::to_string(equity);
                output_string += "},";
                count += 1;
                output_file << output_string << std::endl;
                seen_flops[flop] = true;
              }
            }
          }
        }
      }
    }
  }
  output_file.close();
}

}  // namespace pokerbot