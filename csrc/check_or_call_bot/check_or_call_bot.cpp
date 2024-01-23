#include "../skeleton/actions.h"
#include "../skeleton/runner.h"
#include "../skeleton/states.h"

#include <random>

using namespace pokerbots::skeleton;

struct CheckOrCallBot {
  void handle_new_round(GameInfoPtr /*game_state*/, RoundStatePtr /*round_state*/, int /*active*/) {
  }

  void handle_round_over(GameInfoPtr /*game_state*/, TerminalStatePtr /*terminal_state*/,
                         int /*active*/) {}

  /*
    @param gameState The GameState object.
    @param round_state The RoundState object.
    @param active Your player's index.
    @return Your action.
  */
  Action get_action(GameInfoPtr /*game_state*/, RoundStatePtr round_state, int active) {
    auto legal_actions = round_state->legal_actions();
    auto my_cards = round_state->hands[active];
    auto board_cards = round_state->deck;
    auto my_stack = round_state->stacks[active];

    // Basic bot that randomly bids or just checks/calls.
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<int> bid_distribution(
        0, my_stack);  // random bid between 0 and my stack

    if (legal_actions.find(Action::Type::BID) != legal_actions.end()) {
      return {Action::Type::BID, bid_distribution(gen)};  // random bid
    }
    if (legal_actions.find(Action::Type::CHECK) != legal_actions.end()) {  // check-call
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
