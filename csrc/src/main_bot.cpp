#include "main_bot.h"
#include <random>

#include "mccfr.h"
#include "ranges_utils.h"

namespace pokerbot {

MainBot::MainBot() : game_(), auctioneer_(), mccfr_(100) {}

void MainBot::handle_new_hand(const GameInfo& /*game_info*/, const RoundStatePtr& /*round_state*/,
                              int /*active*/) {
  std::fill(ranges_.begin(), ranges_.end(), Range());
  // int my_bankroll = game_state->bankroll;  // the total number of chips you've gained or lost from the beginning of the game to the start of this round
  // float game_clock = game_state->game_clock;  // the total number of seconds your bot has left to play this game
  // int hand_num = game_state->hand_num;  // the hand from 1 to State.NUM_HANDS
  // auto my_cards = round_state->hands[active];  // your cards
  // bool big_blind = (active == 1);  // true if you are the big blind
}

void MainBot::handle_hand_over(const GameInfo& /*game_info*/,
                               const TerminalStatePtr& /*terminal_state*/, int /*active*/) {
  // int my_delta = terminal_state->deltas[active];  // your bankroll change from this round
  // auto previous_state = std::static_pointer_cast<const RoundState>(terminal_state->previous_state);  // RoundState before payoffs
  // auto my_cards = previous_state->hands[active];  // your cards
  // auto opp_cards = previous_state->hands[1-active];  // opponent's cards or "" if not revealed
}

Action MainBot::get_action(const GameInfo& /*game_info*/, const RoundStatePtr& round_state,
                           int active) {
  const auto legal_actions = round_state->legal_actions();
  const auto& my_cards = round_state->hands[active];

  Hand hero_hand(my_cards[0] + my_cards[1] + my_cards[2]);

  if (ranges::contains(legal_actions, Action::Type::BID)) {
    /// Auction
    const float time_budget_ms = 1;  // FIXME
    const auto bid =
        auctioneer_.get_bid(ranges_[active], ranges_[1 - active], round_state->board_cards,
                            Hand(my_cards[0] + my_cards[1]), round_state->pot(), time_budget_ms);
    return {Action::Type::BID, bid};
  }
  if (round_state->bids[active].has_value() && round_state->bids[1 - active].has_value()) {
    /// Right after auction
    const float time_budget_ms = 1;  // FIXME
    auctioneer_.receive_bid(ranges_[1 - active], *round_state->bids[1 - active],
                            *round_state->bids[active], game_, round_state->board_cards,
                            round_state->pot(), time_budget_ms);
  }

  const float time_budget_ms = 5;  // FIXME
  auto strategy = mccfr_.solve(ranges_, round_state, active, time_budget_ms);

  // TODO UPDATE RANGE here..

  std::vector<float> hand_strat;
  for (unsigned a = 0; a < strategy.num_actions_; ++a) {
    hand_strat.push_back(strategy(hero_hand.index(), a));
  }
  // HACK -> TODO should sample here
  auto idx_action = ranges::argmax(hand_strat);
  auto action = round_state->legal_actions()[idx_action];

  // FIXME -> This is bad.. we don't know which action MCCFR returns
  if (action == Action::Type::RAISE) {
    auto raise_bounds = round_state->raise_bounds();
    return {Action::Type::RAISE, raise_bounds[0]};
  } else {
    return {action};
  }
}

}  // namespace pokerbot
