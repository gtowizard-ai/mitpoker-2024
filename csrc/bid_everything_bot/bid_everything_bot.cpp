#include "../skeleton/actions.h"
#include "../skeleton/runner.h"
#include "../skeleton/states.h"

using namespace pokerbots::skeleton;

struct BidEverythingBot {
  void handle_new_round(const GameInfoPtr& /*game_state*/, const RoundStatePtr& /*round_state*/,
                        int /*active*/) {}

  void handle_round_over(const GameInfoPtr& /*game_state*/,
                         const TerminalStatePtr& /*terminal_state*/, int /*active*/) {}

  /*
    @param gameState The GameState object.
    @param round_state The RoundState object.
    @param active Your player's index.
    @return Your action.
  */
  Action get_action(const GameInfoPtr& /*game_state*/, const RoundStatePtr& round_state,
                    int active) {
    auto legal_actions = round_state->legal_actions();
    auto min_stack = round_state->min_stack();

    // Basic bot that check/call preflop, then bids everything on the flop
    // to win the auction and then goes all-in
    if (legal_actions.find(Action::Type::BID) != legal_actions.end()) {
      return {Action::Type::BID, min_stack};
    }
    if (round_state->street == 3) {
      return {Action::Type::RAISE, min_stack};
    }
    if (legal_actions.find(Action::Type::CHECK) != legal_actions.end()) {
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
  runBot<BidEverythingBot>(host, port);
  return 0;
}
