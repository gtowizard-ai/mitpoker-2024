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
    const auto low_rank = cards[2].to_string()[0];
    const auto mid_rank = cards[1].to_string()[0];
    const auto high_rank = cards[0].to_string()[0];

    if (cards[0].suit() == cards[1].suit() && cards[1].suit() == cards[2].suit()) {
      // Monotone
      return fmt::format("{}c{}c{}c", high_rank, mid_rank, low_rank);
    } else if (cards[0].rank() == cards[1].rank() &&
               (cards[0].suit() == cards[2].suit() || cards[1].suit() == cards[2].suit())) {
      // Suited board on paired board
      return fmt::format("{}c{}d{}c", high_rank, mid_rank, low_rank);
    } else if (cards[1].rank() == cards[2].rank() &&
               (cards[0].suit() == cards[1].suit() || cards[0].suit() == cards[2].suit())) {
      // Suited board on paired board
      return fmt::format("{}c{}c{}d", high_rank, mid_rank, low_rank);
    } else if (cards[0].suit() == cards[1].suit()) {
      return fmt::format("{}c{}c{}d", high_rank, mid_rank, low_rank);
    } else if (cards[0].suit() == cards[2].suit()) {
      // Suited Low High
      return fmt::format("{}c{}d{}c", high_rank, mid_rank, low_rank);
    } else if (cards[1].suit() == cards[2].suit()) {
      // Suited Middle High
      return fmt::format("{}c{}d{}d", high_rank, mid_rank, low_rank);
    } else {
      // Rainbow
      return fmt::format("{}c{}d{}h", high_rank, mid_rank, low_rank);
    }
  }

  static std::vector<int> find_mapping(const std::vector<card_t>& isomorphic_cards,
                                       const std::vector<card_t>& cards) {
    // Only 24 possible mappings in total (4 factorial) so we iterate over all of
    // them until we find the right one.
    int mapping[] = {0, 1, 2, 3};
    do {
      bool ok = true;
      for (unsigned i = 0; i < isomorphic_cards.size(); ++i) {
        auto prev_suit = suit_of_card(isomorphic_cards[i]);
        auto isomorph_suit = mapping[prev_suit];
        auto isomorph_card = make_card(rank_of_card(isomorphic_cards[i]), isomorph_suit);

        // If flop check all flop cards, else check cards until now.
        // this works because we always keep the rounds ordering with isomorphism
        auto end_index = i < 3 ? cards.begin() + 3 : cards.begin() + (i + 1);
        end_index = end_index >= cards.end() ? cards.end() : end_index;
        if (std::find(cards.begin(), end_index, isomorph_card) == end_index) {
          ok = false;
          break;
        }
      }
      if (ok) {
        std::vector<int> mappings(mapping, mapping + MAX_SUITS);
        return mappings;
      }
    } while (std::next_permutation(mapping, mapping + MAX_SUITS));

    throw std::invalid_argument(
        fmt::format("No suit mapping from isomorphic_cards '{}' to cards '{}'",
                    Card::to_string(isomorphic_cards), Card::to_string(cards)));
  }
};

}  // namespace pokerbot
