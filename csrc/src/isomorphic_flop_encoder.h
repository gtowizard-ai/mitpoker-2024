#pragma once
#include <fmt/format.h>
#include <algorithm>
#include "card.h"

namespace pokerbot {

class IsomorphicFlopEncoder {
 public:
  static unsigned constexpr NUM_FLOPS = 1755;

  // Custom/ugly isomorphic flop encoder
  static std::string to_isomorphic_flop(const std::vector<card_t>& raw_cards) {
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

}  // namespace pokerbot
