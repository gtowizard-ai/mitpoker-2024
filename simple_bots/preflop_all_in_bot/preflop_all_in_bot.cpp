#include "src/actions.h"
#include "src/ranges_utils.h"
#include "src/runner.h"
#include "src/states.h"

using namespace pokerbot;

struct PreflopAllInBot {
  void handle_new_hand(const GameInfo& /*game_info*/, const RoundStatePtr& /*round_state*/,
                       int /*active*/) {}

  void handle_hand_over(const GameInfo& /*game_info*/, const TerminalStatePtr& /*terminal_state*/,
                        int /*active*/) {}

  Action get_action(const GameInfo& /*game_info*/, const RoundStatePtr& round_state,
                    int /*active*/) {
    auto legal_actions = round_state->legal_actions();
    auto min_stack = round_state->effective_stack();

    if (legal_actions.size() == 1) {
      // Bid and check once we're all-in
      return {legal_actions.front()};
    }

    if (ranges::contains(legal_actions, Action::Type::RAISE)) {
      return {Action::Type::RAISE, STARTING_STACK};
    }
    if (ranges::contains(legal_actions, Action::Type::CALL)) {
      return {Action::Type::CALL};
    }
    throw std::runtime_error(fmt::format("Shouldn't have reached here: {}", legal_actions.size()));
  }
};

/*
  Main program for running a C++ pokerbot.
*/
int main(int argc, char* argv[]) {
  auto [host, port] = parseArgs(argc, argv);
  runBot<PreflopAllInBot>(host, port);
  return 0;
}
