#include "main_bot.h"
#include <fmt/ranges.h>
#include "ranges_utils.h"

namespace pokerbot {

MainBot::MainBot()
    : game_(), auctioneer_(), cfr_(game_), time_manager_(), gen_(std::random_device{}()) {}

Action MainBot::sample_action_and_update_range(const RoundState& state, const Hand& hand,
                                               const int hero_id, const HandActionsValues& strategy,
                                               const float min_prob_sampling) {
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

void MainBot::handle_new_hand(const GameInfo& /*game_info*/, const RoundStatePtr& /*state*/,
                              int /*active*/) {
  std::fill(ranges_.begin(), ranges_.end(), Range());
}

void MainBot::handle_hand_over(const GameInfo& /*game_info*/,
                               const TerminalStatePtr& /*terminal_state*/, int /*active*/) {
  // int my_delta = terminal_state->deltas[active];  // your bankroll change from this round
  // auto previous_state = std::static_pointer_cast<const RoundState>(terminal_state->previous_state);  // RoundState before payoffs
  // auto my_cards = previous_state->hands[active];  // your cards
  // auto opp_cards = previous_state->hands[1-active];  // opponent's cards or "" if not revealed
}

Action MainBot::get_action(const GameInfo& game_info, const RoundStatePtr& state, int active) {
  const auto legal_actions = state->legal_actions();

  time_manager_.update_on_get_action(game_info);

  const Hand hero_hand(state->hands[active]);

  // TODO - Need to update ranges on new board cards

  if (ranges::contains(legal_actions, Action::Type::BID)) {
    /// Auction
    const float time_budget_ms = 1;  // FIXME
    const auto bid =
        auctioneer_.get_bid(ranges_[active], ranges_[1 - active], game_, state->board_cards,
                            hero_hand, state->pot(), time_budget_ms);
    fmt::print("Bidding {} in {}\n", bid, state->pot());
    return {Action::Type::BID, bid};
  }
  if (state->bids[active].has_value() && state->bids[1 - active].has_value()) {
    /// Right after auction
    /// NB - Careful this is called on *every action* after auction happened
    const float time_budget_ms = 1;  // FIXME
    auctioneer_.receive_bid(ranges_[active], ranges_[1 - active], *state->bids[active],
                            *state->bids[1 - active], game_, state->board_cards, state->pot(),
                            time_budget_ms);
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

  Action sampled_action;
  if (state->round() == round::PREFLOP && active == SB_POS &&
      state->bets[1 - active] == BIG_BLIND) {
    if (!preflop_sb_cached_strategy_.has_value()) {
      // Solve with a larger time limit (computed once)
      fmt::print("Solving root preflop node for 100ms \n");
      cfr_.solve(ranges_, state, active, 100);
      preflop_sb_cached_strategy_ = cfr_.strategy();
    }
    sampled_action =
        sample_action_and_update_range(*state, hero_hand, active, *preflop_sb_cached_strategy_);
  } else {
    // Solve..
    // FIXME - On flop/turn, CFVs are calculated as if showdown will be held with the current board
    // (i.e., no more chance cards are dealt)
    const auto time_budget_ms = time_manager_.time_ms_allowed_for_cfr(game_info);
    fmt::print("{:.2f} ms allocated for solving with CFR \n", time_budget_ms);
    cfr_.solve(ranges_, state, active, time_budget_ms);
    sampled_action = sample_action_and_update_range(*state, hero_hand, active, cfr_.strategy());
  }

  // TODO - Update Villain range somehow..

  return sampled_action;
}

}  // namespace pokerbot
