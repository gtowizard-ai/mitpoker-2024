#include "main_bot.h"
#include <random>
#include "ranges_utils.h"

namespace pokerbot {

MainBot::MainBot() : game_(), auctioneer_() {}

void MainBot::handle_new_hand(const GameInfo& /*game_info*/, const RoundStatePtr& /*round_state*/,
                              int /*active*/) {
  hero_range_ = Range();
  opponent_range_ = Range();
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
  const auto legal_actions = round_state->legal_actions();  // the actions you are allowed to take
  const auto& my_cards = round_state->hands[active];        // your cards

  if (ranges::contains(legal_actions, Action::Type::BID)) {
    /// Auction
    const float time_budget_ms = 1;  // FIXME
    const auto bid =
        auctioneer_.get_bid(hero_range_, opponent_range_, round_state->board_cards,
                            Hand(my_cards[0] + my_cards[1]), round_state->pot(), time_budget_ms);
    return {Action::Type::BID, bid};
  }
  if (round_state->bids[active].has_value() && round_state->bids[1 - active].has_value()) {
    /// Right after auction
    const float time_budget_ms = 1;  // FIXME
    auctioneer_.receive_bid(opponent_range_, *round_state->bids[1 - active],
                            *round_state->bids[active], game_, round_state->board_cards,
                            round_state->pot(), time_budget_ms);
  }

  /// Make an action
  if (ranges::contains(legal_actions, Action::Type::CHECK)) {
    // check-call
    return {Action::Type::CHECK};
  }
  return {Action::Type::CALL};
}

}  // namespace pokerbot
