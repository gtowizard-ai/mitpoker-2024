#include "node_evaluation.h"
#include "poker_hand.h"

namespace pokerbot {

TerminalValue::TerminalValue(const Game& game, const std::vector<card_t>& board,
                             const std::vector<Range>& ranges, const unsigned player_id)
    : game_(game),
      board_(board),
      ranges_(ranges),
      player_id_(player_id),
      sum_(0),
      terminal_values_(ranges_[player_id].num_hands(), 0) {
  // ToDo: Check if the node is river
  for (int player = 0; player < num_players_; player++) {
    player_strengthses_[player].reserve(ranges_[player].num_hands());

    for (unsigned index = 0; index < ranges_[player].num_hands(); index++) {
      std::vector<card_t> hand(board_);
      for (const auto card : game_.hands(ranges_[player].num_cards)[index].cards) {
        hand.push_back(card);
      }
      player_strengthses_[player].push_back(HandStrength{index, PokerHand(hand).evaluate()});
    }
    std::sort(player_strengthses_[player].begin(), player_strengthses_[player].end());
  }

  for (int index = 0; index < ranges[1 - player_id].num_hands(); index++) {
    sum_ += ranges[1 - player_id].range[index];
  }
}

void TerminalValue::compute_terminal_values(const Payoff& payoff) {
  OutcomeProb sum_probs = {sum_, 0, 0};
  std::fill(terminal_values_.begin(), terminal_values_.end(), 0);

  unsigned j = 0;
  for (unsigned index = 0; index < player_strengthses_[player_id_].size(); index++) {
    if (index > 0 && player_strengthses_[player_id_][index].strength ==
                         player_strengthses_[player_id_][index - 1].strength) {
      terminal_values_[index] = terminal_values_[index - 1];
      continue;
    }

    sum_probs.win += sum_probs.tie;
    sum_probs.tie = 0;

    double tie = 0, win = 0;
    for (; j < player_strengthses_[1 - player_id_].size(); j++) {
      if (player_strengthses_[1 - player_id_][j].strength >
          player_strengthses_[player_id_][index].strength) {
        break;
      }

      if (player_strengthses_[1 - player_id_][j].strength ==
          player_strengthses_[player_id_][index].strength) {
        tie += ranges_[1 - player_id_].range[j];
      } else {
        win += ranges_[1 - player_id_].range[j];
      }
    }

    sum_probs.tie = tie;
    sum_probs.win += win;
    sum_probs.lose -= tie + win;

    // compute the value
    terminal_values_[index] = static_cast<float>(
        payoff.lose * sum_probs.lose + payoff.tie * sum_probs.tie + payoff.win * sum_probs.win);
  }
}

}  // namespace pokerbot