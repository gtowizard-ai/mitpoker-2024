#include <random>
#include "../skeleton/actions.h"
#include "../skeleton/runner.h"
#include "../skeleton/states.h"
#include "../src/ranges_utils.h"

using namespace pokerbots::skeleton;

struct CheckOrCallBot {
  void handle_new_round(const GameInfo& /*game_state*/, const RoundStatePtr& /*round_state*/,
                        int /*active*/) {}

  void handle_round_over(const GameInfo& /*game_state*/, const TerminalStatePtr& /*terminal_state*/,
                         int /*active*/) {}

  /*
    @param game_state The GameState object.
    @param round_state The RoundState object.
    @param active Your player's index.
    @return Your action.
  */
  Action get_action(const GameInfo& /*game_state*/, const RoundStatePtr& round_state, int active) {
    auto legal_actions = round_state->legal_actions();
    auto my_stack = round_state->stacks[active];

    // Basic bot that randomly bids or just checks/calls.
    std::random_device rd;
    std::mt19937 gen(rd());
    // random bid between 0 and my stack
    std::uniform_int_distribution<int> bid_distribution(0, my_stack);

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
  runBot<CheckOrCallBot>(host, port);
  return 0;
}
