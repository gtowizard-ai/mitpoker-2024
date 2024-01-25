#include "node_evaluation.h"
#include "poker_hand.h"

namespace pokerbot {

/// Computes the expected payoff of the river nodes
void compute_terminal_values(const Game& game, const std::vector<card_t>& board,
                             const Payoff& payoff, const std::vector<Range>& ranges,
                             const unsigned player_id) {
  // ToDo: Check if the node is river
  std::vector<HandStrength> opp_strengths(ranges[1 - player_id].num_hands());
  std::vector<HandStrength> hero_strengths(ranges[player_id].num_hands());

  for (unsigned index = 0; index < opp_strengths.size(); index++) {
    std::vector<card_t> opp_hand(board);
    for (const auto card : game.hands(ranges[1 - player_id].num_cards)[index].cards) {
      opp_hand.push_back(card);
    }
    opp_strengths[index] = HandStrength{index, PokerHand(opp_hand).evaluate()};
  }

  for (unsigned index = 0; index < hero_strengths.size(); index++) {
    std::vector<card_t> hero_hand(board);
    for (const auto card : game.hands(ranges[player_id].num_cards)[index].cards) {
      hero_hand.push_back(card);
    }
    hero_strengths[index] = HandStrength{index, PokerHand(hero_hand).evaluate()};
  }

  // Sort the hands of the Hero and the Villian
  // ToDo: Keep sorted strength in cache
  std::sort(opp_strengths.begin(), opp_strengths.end());
  std::sort(hero_strengths.begin(), hero_strengths.end());

  std::vector<float> terminal_values(hero_strengths.size());

  OutcomeProb sum_probs = {0, 0, 0};
  for (unsigned index = 0; index < ranges[1 - player_id].num_hands(); index++) {
    sum_probs.lose += ranges[1 - player_id].range[index];
  }

  unsigned j = 0;
  for (unsigned index = 0; index < hero_strengths.size(); index++) {
    if (index > 0 && hero_strengths[index].strength == hero_strengths[index - 1].strength) {
      terminal_values[index] = terminal_values[index - 1];
      continue;
    }

    sum_probs.win += sum_probs.tie;
    sum_probs.tie = 0;

    double tie = 0, win = 0;
    for (; j < opp_strengths.size(); j++) {
      if (opp_strengths[j].strength > hero_strengths[index].strength) {
        break;
      }

      if (opp_strengths[j].strength == hero_strengths[index].strength) {
        tie += ranges[1 - player_id].range[j];
      } else {
        win += ranges[1 - player_id].range[j];
      }
    }

    sum_probs.tie = tie;
    sum_probs.win += win;
    sum_probs.lose -= tie + win;

    // compute the value
    terminal_values[index] = static_cast<float>(
        payoff.lose * sum_probs.lose + payoff.tie * sum_probs.tie + payoff.win * sum_probs.win);
  }
}

}  // namespace pokerbot