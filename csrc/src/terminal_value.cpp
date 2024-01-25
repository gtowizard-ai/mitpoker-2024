#include "terminal_value.h"
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

    for (int index = 0; index < ranges_[player].num_hands(); index++) {
      std::vector<card_t> board_hand(board_);
      auto& hand = game_.hands(ranges_[player].num_cards)[index];

      // blockers
      if (hand.collides_with(board_hand)) {
        player_strengthses_[player].push_back(HandStrength{-1, 0});
        continue;
      }

      for (unsigned i = 0; i < hand.num_cards(); i++) {
        board_hand.push_back(hand.cards[i]);
      }

      player_strengthses_[player].push_back(HandStrength{index, PokerHand(board_hand).evaluate()});
    }
    std::sort(player_strengthses_[player].begin(), player_strengthses_[player].end());
  }

  for (int index = 0; index < ranges[1 - player_id].num_hands(); index++) {
    // ToDo: remove ranges with blocker with board, if needed
    sum_ += ranges[1 - player_id].range[index];
  }
}

std::vector<float> TerminalValue::compute_terminal_values(const Payoff& payoff) {
  // ToDo: Blockers of the Opp and Hero
  OutcomeProb sum_probs = {sum_, 0, 0};
  std::fill(terminal_values_.begin(), terminal_values_.end(), 0);

  unsigned j = 0;
  for (unsigned index = 0; index < player_strengthses_[player_id_].size(); index++) {
    // Hero blocker
    if (player_strengthses_[player_id_][index].index == -1) {
      continue;
    }

    const auto terminal_values_index = player_strengthses_[player_id_][index].index;

    if (index > 0 && player_strengthses_[player_id_][index].strength ==
                         player_strengthses_[player_id_][index - 1].strength) {
      terminal_values_[terminal_values_index] =
          terminal_values_[player_strengthses_[player_id_][index - 1].index];
      continue;
    }

    sum_probs.win += sum_probs.tie;
    sum_probs.tie = 0;

    double tie = 0, win = 0;
    for (; j < player_strengthses_[1 - player_id_].size(); j++) {
      // ToDo: If the range for the opp blocker hand is not 0, handle it here
      const auto opp_index = player_strengthses_[1 - player_id_][j].index;

      if (player_strengthses_[1 - player_id_][j].strength >
          player_strengthses_[player_id_][index].strength) {
        break;
      }

      if (player_strengthses_[1 - player_id_][j].strength ==
          player_strengthses_[player_id_][index].strength) {
        tie += ranges_[1 - player_id_].range[opp_index];
      } else {
        win += ranges_[1 - player_id_].range[opp_index];
      }
    }

    sum_probs.tie = tie;
    sum_probs.win += win;
    sum_probs.lose -= tie + win;

    // compute the value
    terminal_values_[terminal_values_index] = static_cast<float>(
        payoff.lose * sum_probs.lose + payoff.tie * sum_probs.tie + payoff.win * sum_probs.win);
  }

  return terminal_values_;
}

}  // namespace pokerbot