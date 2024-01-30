#include <random>
#include "../src/actions.h"
#include "../src/runner.h"
#include "../src/states.h"

using namespace pokerbot;

struct UniformRandomBot {
  void handle_new_hand(const GameInfo& /*game_info*/, const RoundStatePtr& /*round_state*/,
                       int /*active*/) {}

  void handle_hand_over(const GameInfo& /*game_info*/, const TerminalStatePtr& /*terminal_state*/,
                        int /*active*/) {}

  Action get_action(const GameInfo& /*game_info*/, const RoundStatePtr& round_state, int active) {
    const auto legal_actions = round_state->legal_actions();
    const auto my_stack = round_state->stacks[active];

    // Basic bot that randomly takes an action
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> action_distribution(0, legal_actions.size() - 1);
    const auto sampled_action = legal_actions[action_distribution(gen)];

    if (sampled_action == Action::Type::BID) {
      // random bid between 0 and my stack
      std::uniform_int_distribution<int> bid_distribution(0, my_stack);
      return {Action::Type::BID, bid_distribution(gen)};
    } else if (sampled_action == Action::Type::RAISE) {
      // random bet between 0 and my stack
      const auto raise_bounds = round_state->raise_bounds();
      std::uniform_int_distribution<int> bet_distribution(raise_bounds[0], raise_bounds[1]);
      return {Action::Type::RAISE, bet_distribution(gen)};
    } else {
      return {sampled_action};
    }
  }
};

/*
  Main program for running a C++ pokerbot.
*/
int main(int argc, char* argv[]) {
  auto [host, port] = parseArgs(argc, argv);
  runBot<UniformRandomBot>(host, port);
  return 0;
}
