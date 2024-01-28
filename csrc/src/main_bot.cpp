#include "main_bot.h"
#include <fmt/ranges.h>
#include "ranges_utils.h"

namespace pokerbot {

MainBot::MainBot() : game_(), auctioneer_(), cfr_(game_), gen_(std::random_device{}()) {}

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

void MainBot::handle_new_hand(const GameInfo& /*game_info*/, const RoundStatePtr& /*state*/,
                              int /*active*/) {
  std::fill(ranges_.begin(), ranges_.end(), Range());
  // int my_bankroll = game_state->bankroll;  // the total number of chips you've gained or lost from the beginning of the game to the start of this round
  // float game_clock = game_state->game_clock;  // the total number of seconds your bot has left to play this game
  // int hand_num = game_state->hand_num;  // the hand from 1 to State.NUM_HANDS
  // auto my_cards = state->hands[active];  // your cards
  // bool big_blind = (active == 1);  // true if you are the big blind
}

void MainBot::handle_hand_over(const GameInfo& /*game_info*/,
                               const TerminalStatePtr& /*terminal_state*/, int /*active*/) {
  // int my_delta = terminal_state->deltas[active];  // your bankroll change from this round
  // auto previous_state = std::static_pointer_cast<const RoundState>(terminal_state->previous_state);  // RoundState before payoffs
  // auto my_cards = previous_state->hands[active];  // your cards
  // auto opp_cards = previous_state->hands[1-active];  // opponent's cards or "" if not revealed
}

Action MainBot::get_action(const GameInfo& /*game_info*/, const RoundStatePtr& state, int active) {
  const auto legal_actions = state->legal_actions();

  if (legal_actions.size() == 1) {
    // Engine keeps asking to take an action even when both players are all-in..
    // Don't waste any time and return
    return Action{state->legal_actions().front()};
  }

  const auto& my_cards = state->hands[active];
  Hand hero_hand(my_cards[0] + my_cards[1] + my_cards[2]);

  if (ranges::contains(legal_actions, Action::Type::BID)) {
    /// Auction
    fmt::print("Bidding.. \n");
    const float time_budget_ms = 1;  // FIXME
    const auto bid =
        auctioneer_.get_bid(ranges_[active], ranges_[1 - active], state->board_cards,
                            Hand(my_cards[0] + my_cards[1]), state->pot(), time_budget_ms);
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

  // FIXME cant afford to do CFR on flop/turn for now
  if (state->round() == round::FLOP || state->round() == round::TURN) {
    if (ranges::contains(legal_actions, Action::Type::CHECK)) {
      // check-call
      return {Action::Type::CHECK};
    }
    return {Action::Type::CALL};
  }

  // Solve..
  const float time_budget_ms = 5;  // FIXME
  cfr_.solve(ranges_, state, active, time_budget_ms);
  const auto sampled_action = sample_action_and_update_range(*state, hero_hand, active);

  // TODO UPDATE RANGE here..

  return sampled_action;
}

}  // namespace pokerbot
