#include "main_bot.h"
#include <fmt/ranges.h>
#include "ranges_utils.h"

namespace pokerbot {

MainBot::MainBot() : game_(), auctioneer_(), cfr_(game_) {}

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
    fmt::print("Bidding.. \n");
    const float time_budget_ms = 1;  // FIXME
    const auto bid =
        auctioneer_.get_bid(ranges_[active], ranges_[1 - active], round_state->board_cards,
                            Hand(my_cards[0] + my_cards[1]), round_state->pot(), time_budget_ms);
    return {Action::Type::BID, bid};
  }
  if (round_state->bids[active].has_value() && round_state->bids[1 - active].has_value()) {
    /// Right after auction
    /// NB - Careful this is called on *every action* after auction happened
    const float time_budget_ms = 1;  // FIXME
    auctioneer_.receive_bid(ranges_[1 - active], *round_state->bids[1 - active],
                            *round_state->bids[active], game_, round_state->board_cards,
                            round_state->pot(), time_budget_ms);
  }

  // FIXME cant afford to do CFR on flop/turn for now
  if (round_state->round() == round::FLOP || round_state->round() == round::TURN) {
    if (ranges::contains(legal_actions, Action::Type::CHECK)) {
      // check-call
      return {Action::Type::CHECK};
    }
    return {Action::Type::CALL};
  }

  const float time_budget_ms = 1;  // FIXME
  const auto strategy = cfr_.solve(ranges_, round_state, active, time_budget_ms);

  // TODO UPDATE RANGE here..

  std::vector<float> hand_strat;
  for (unsigned a = 0; a < strategy.num_actions_; ++a) {
    hand_strat.push_back(strategy(hero_hand.index(), a));
  }
  // HACK -> TODO should sample here
  const auto idx_action = ranges::argmax(hand_strat);
  const auto action = cfr_.legal_actions()[idx_action];

  fmt::print("Best action for hand {} on {} is {} (Strat={})\n", hero_hand.to_string(),
             Card::to_string(round_state->board_cards), action.to_string(), hand_strat);
  return action;
}

}  // namespace pokerbot
