#include <random>
#include "../src/actions.h"
#include "../src/constants.h"
#include "../src/ranges_utils.h"
#include "../src/runner.h"
#include "../src/states.h"

using namespace pokerbot;

struct MainBot {
  /*
    Called when a new round starts. Called NUM_ROUNDS times.

    @param game_state The GameState object.
    @param round_state The RoundState object.
    @param active Your player's index.
  */
  void handle_new_round(const GameInfo& /*game_state*/, const RoundStatePtr& /*round_state*/,
                        int /*active*/) {
    // int my_bankroll = game_state->bankroll;  // the total number of chips you've gained or lost from the beginning of the game to the start of this round
    // float game_clock = game_state->game_clock;  // the total number of seconds your bot has left to play this game
    // int round_num = game_state->round_num;  // the round number from 1 to State.NUM_ROUNDS
    // auto my_cards = round_state->hands[active];  // your cards
    // bool big_blind = (active == 1);  // true if you are the big blind
  }

  /*
    Called when a round ends. Called NUM_ROUNDS times.

    @param game_state The GameState object.
    @param terminal_state The TerminalState object.
    @param active Your player's index.
  */
  void handle_round_over(const GameInfo& /*game_state*/, const TerminalStatePtr& /*terminal_state*/,
                         int /*active*/) {
    // int my_delta = terminal_state->deltas[active];  // your bankroll change from this round
    // auto previous_state = std::static_pointer_cast<const RoundState>(terminal_state->previous_state);  // RoundState before payoffs
    // auto my_cards = previous_state->hands[active];  // your cards
    // auto opp_cards = previous_state->hands[1-active];  // opponent's cards or "" if not revealed
  }

  /*
    Where the magic happens - your code should implement this function.
    Called any time the engine needs an action from your bot.

    @param game_state The GameState object.
    @param round_state The RoundState object.
    @param active Your player's index.
    @return Your action.
  */
  Action get_action(const GameInfo& /*game_state*/, const RoundStatePtr& round_state, int active) {
    // May be useful, but you can choose to not use.
    auto legal_actions = round_state->legal_actions();  // the actions you are allowed to take
    auto my_cards = round_state->hands[active];         // your cards
    auto board_cards = round_state->deck;               // the board cards
    int my_pip =
        round_state->pips
            [active];  // the number of chips you have contributed to the pot this round of betting
    int opp_pip = round_state->pips[1 - active];
    // the number of chips your opponent has contributed to the pot this round of betting
    int my_stack = round_state->stacks[active];  // the number of chips you have remaining
    int opp_stack =
        round_state->stacks[1 - active];  // the number of chips your opponent has remaining
    std::optional<int> myBid =
        round_state->bids[active];  // How much you bid previously (available only after auction)
    std::optional<int> oppBid =
        round_state
            ->bids[1 - active];  // How much opponent bid previously (available only after auction)
    int continue_cost = opp_pip - my_pip;  // the number of chips needed to stay in the pot
    int my_contribution =
        STARTING_STACK - my_stack;  // the number of chips you have contributed to the pot
    int opp_contribution =
        STARTING_STACK - opp_stack;  // the number of chips your opponent has contributed to the pot
    int min_cost = 0;
    int max_cost = 0;
    if (ranges::contains(legal_actions, Action::Type::RAISE)) {
      auto raise_bounds =
          round_state
              ->raise_bounds();  // the smallest and largest numbers of chips for a legal bet/raise
      min_cost = raise_bounds[0] - my_pip;  // the cost of a minimum bet/raise
      max_cost = raise_bounds[1] - my_pip;  // the cost of a maximum bet/raise
    }

    // Basic bot that randomly bids or just checks/calls.
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<int> bid_distribution(
        0, my_stack);  // random bid between 0 and my stack

    if (ranges::contains(legal_actions, Action::Type::BID)) {
      return {Action::Type::BID, bid_distribution(gen)};  // random bid
    }
    if (ranges::contains(legal_actions, Action::Type::CHECK)) {
      // check-call
      return {Action::Type::CHECK};
    }
    return {Action::Type::CALL};
  }
};

/*
  Main program for running a C++ pokerbot.
*/
int main(int argc, char* argv[]) {
  auto [host, port] = parseArgs(argc, argv);
  runBot<MainBot>(host, port);
  return 0;
}
