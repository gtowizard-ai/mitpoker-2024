#include "main_bot.h"
#include <fmt/ranges.h>
#include "ranges_utils.h"

namespace pokerbot {

MainBot::MainBot()
    : game_(), auctioneer_(), cfr_(game_), time_manager_(), gen_(std::random_device{}()) {}

Action MainBot::sample_action_and_update_range(const RoundState& state, const Hand& hand,
                                               const int hero_id, const float min_prob_sampling) {
  const auto& strategy = cfr_.strategy();

  // Get strategy for hand
  std::vector<float> probs;
  probs.reserve(strategy.num_actions_);
  for (unsigned a = 0; a < strategy.num_actions_; ++a) {
    probs.push_back(strategy(hand.index(), a));
  }
  // Don't sample if prob is too low due to non-convergence
  for (auto& val : probs) {
    if (val < min_prob_sampling) {
      val = 0;
    }
  }

  std::discrete_distribution<> dis(probs.begin(), probs.end());
  const int sampled_idx = dis(gen_);
  const auto sampled_action = cfr_.legal_actions()[sampled_idx];
  fmt::print("{} - {} - Sampled Action {} on {} based on strategy = {} \n",
             state.round().to_string(), hand.to_string(), sampled_action.to_string(),
             Card::to_string(state.board_cards), probs);

  for (hand_t i = 0; i < ranges_[hero_id].num_hands(); ++i) {
    const auto action_prob = strategy(i, sampled_idx);
    ranges_[hero_id].range[i] = ranges_[hero_id].range[i] * action_prob;
  }

  return sampled_action;
}

void MainBot::handle_new_hand(const GameInfo& game_info, const RoundStatePtr& /*state*/,
                              int /*active*/) {
  std::fill(ranges_.begin(), ranges_.end(), Range());
  fmt::print("Time Remaining:: {}\n", game_info.game_clock);
  // fmt::print("Time spent --> {}\n", time_manager_.total_actions_per_round_);
}

void MainBot::handle_hand_over(const GameInfo& /*game_info*/,
                               const TerminalStatePtr& /*terminal_state*/, int /*active*/) {
  // int my_delta = terminal_state->deltas[active];  // your bankroll change from this round
  // auto previous_state = std::static_pointer_cast<const RoundState>(terminal_state->previous_state);  // RoundState before payoffs
  // auto my_cards = previous_state->hands[active];  // your cards
  // auto opp_cards = previous_state->hands[1-active];  // opponent's cards or "" if not revealed
}

Action MainBot::get_action(const GameInfo& game_info, const RoundStatePtr& state,
                           const int active) {
  const auto start_time = std::chrono::high_resolution_clock::now();
  time_manager_.update_action(state);

  const auto action = get_action(state, active, time_manager_.get_time_budget_ms(game_info, state));

  const auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::high_resolution_clock::now() - start_time)
                               .count();
  time_manager_.update_time(state, static_cast<float>(duration_ms));

  return action;
}

Action MainBot::get_action(const RoundStatePtr& state, const int active,
                           const float time_budget_ms) {
  const auto legal_actions = state->legal_actions();

  const Hand hero_hand(state->hands[active]);

  // TODO - Need to update ranges on new board cards

  if (ranges::contains(legal_actions, Action::Type::BID)) {
    /// Auction
    const auto bid = auctioneer_.get_bid(ranges_[active], ranges_[1 - active], game_,
                                         state->board_cards, hero_hand, state->pot());
    fmt::print("Bidding {} in {}\n", bid, state->pot());
    return {Action::Type::BID, bid};
  }

  if (state->bids[active].has_value() && state->bids[1 - active].has_value()) {
    /// Right after auction
    /// NB - Careful this is called on *every action* after auction happened

    auctioneer_.receive_bid(ranges_[active], ranges_[1 - active], *state->bids[active],
                            *state->bids[1 - active], game_, state->board_cards, state->pot());
  }

  // NB: Put this constraint after checking if only legal action is bid!
  if (legal_actions.size() == 1) {
    // Engine keeps asking to take an action even when both players are all-in..
    // Don't waste any time and return
    return Action{state->legal_actions().front()};
  }

  if (state->round() == round::FLOP && state->bets[1 - active] == 0 && active == BB_POS) {
    fmt::print("OOP Flop - Checking 100% of the time \n");
    return Action{Action::Type::CHECK};
  }
  if (state->round() == round::TURN && state->bets[1 - active] == 0 && active == BB_POS &&
      hero_hand.num_cards() == 2) {
    fmt::print("OOP Turn with 2 cards - Checking 100% of the time \n");
    return Action{Action::Type::CHECK};
  }

  // Solve..
  // FIXME - On flop/turn, CFVs are calculated as if showdown will be held with the current board
  // (i.e., no more chance cards are dealt)
  fmt::print("{:.2f} ms allocated for solving with CFR \n", time_budget_ms);
  cfr_.solve(ranges_, state, active, time_budget_ms);
  const auto sampled_action = sample_action_and_update_range(*state, hero_hand, active);

  // TODO - Update Villain range somehow..

  return sampled_action;
}

}  // namespace pokerbot
