#include "../src/actions.h"
#include "../src/ranges_utils.h"
#include "../src/runner.h"
#include "../src/states.h"

using namespace pokerbot;

struct BidEverythingBot {
  void handle_new_hand(const GameInfo& /*game_info*/, const RoundStatePtr& /*round_state*/,
                       int /*active*/) {}

  void handle_hand_over(const GameInfo& /*game_info*/, const TerminalStatePtr& /*terminal_state*/,
                        int /*active*/) {}

  Action get_action(const GameInfo& /*game_info*/, const RoundStatePtr& round_state,
                    int /*active*/) {
    auto legal_actions = round_state->legal_actions();
    auto min_stack = round_state->effective_stack();

    // Basic bot that check/call preflop, then bids everything on the flop
    // to win the auction and then goes all-in
    if (ranges::contains(legal_actions, Action::Type::BID)) {
      return {Action::Type::BID, min_stack};
    }
    if (round_state->round() == round::FLOP) {
      return {Action::Type::RAISE, min_stack};
    }
    if (ranges::contains(legal_actions, Action::Type::CHECK)) {
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
