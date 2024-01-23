#include <skeleton/actions.h>
#include <skeleton/constants.h>
#include <skeleton/runner.h>
#include <skeleton/states.h>

#include <random>

using namespace pokerbots::skeleton;

struct Bot {
  /*
    Called when a new round starts. Called NUM_ROUNDS times.

    @param gameState The GameState object.
    @param roundState The RoundState object.
    @param active Your player's index.
  */
  void handleNewRound(GameInfoPtr gameState, RoundStatePtr roundState, int active) {
    // int myBankroll = gameState->bankroll;  // the total number of chips you've gained or lost from the beginning of the game to the start of this round
    // float gameClock = gameState->gameClock;  // the total number of seconds your bot has left to play this game
    // int roundNum = gameState->roundNum;  // the round number from 1 to State.NUM_ROUNDS
    // auto myCards = roundState->hands[active];  // your cards
    // bool bigBlind = (active == 1);  // true if you are the big blind
  }

  /*
    Called when a round ends. Called NUM_ROUNDS times.

    @param gameState The GameState object.
    @param terminalState The TerminalState object.
    @param active Your player's index.
  */
  void handleRoundOver(GameInfoPtr gameState, TerminalStatePtr terminalState, int active) {
    // int myDelta = terminalState->deltas[active];  // your bankroll change from this round
    // auto previousState = std::static_pointer_cast<const RoundState>(terminalState->previousState);  // RoundState before payoffs
    // int street = previousState->street;  // 0, 3, 4, or 5 representing when this round ended
    // auto myCards = previousState->hands[active];  // your cards
    // auto oppCards = previousState->hands[1-active];  // opponent's cards or "" if not revealed
  }
  
  /*
    Where the magic happens - your code should implement this function.
    Called any time the engine needs an action from your bot.

    @param gameState The GameState object.
    @param roundState The RoundState object.
    @param active Your player's index.
    @return Your action.
  */
  Action getAction(GameInfoPtr gameState, RoundStatePtr roundState, int active) {

    // May be useful, but you can choose to not use.
    auto legalActions = roundState->legalActions();  // the actions you are allowed to take
    int street = roundState->street;  // 0, 3, 4, or 5 representing pre-flop, flop, turn, or river respectively
    auto myCards = roundState->hands[active];  // your cards
    auto boardCards = roundState->deck;  // the board cards
    int myPip = roundState->pips[active];  // the number of chips you have contributed to the pot this round of betting
    int oppPip = roundState->pips[1-active];  // the number of chips your opponent has contributed to the pot this round of betting
    int myStack = roundState->stacks[active];  // the number of chips you have remaining
    int oppStack = roundState->stacks[1-active];  // the number of chips your opponent has remaining
    std::optional<int> myBid = roundState->bids[active];       // How much you bid previously (available only after auction)
    std::optional<int> oppBid = roundState->bids[1 - active];  // How much opponent bid previously (available only after auction)
    int continueCost = oppPip - myPip;  // the number of chips needed to stay in the pot
    int myContribution = STARTING_STACK - myStack;  // the number of chips you have contributed to the pot
    int oppContribution = STARTING_STACK - oppStack;  // the number of chips your opponent has contributed to the pot
    int minCost = 0;
    int maxCost = 0;
    if (legalActions.find(Action::Type::RAISE) != legalActions.end()) {
      auto raiseBounds = roundState->raiseBounds();  // the smallest and largest numbers of chips for a legal bet/raise
      minCost = raiseBounds[0] - myPip;  // the cost of a minimum bet/raise
      maxCost = raiseBounds[1] - myPip;  // the cost of a maximum bet/raise
    }

    // Basic bot that randomly bids or just checks/calls.
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<int> bid_distribution(0, myStack); // random bid between 0 and my stack

    if (legalActions.find(Action::Type::BID) != legalActions.end()) {  
      return {Action::Type::BID, bid_distribution(gen)}; // random bid
    }
    if (legalActions.find(Action::Type::CHECK) != legalActions.end()) {  // check-call
      return {Action::Type::CHECK};
    }
    return {Action::Type::CALL};
  }
};

/*
  Main program for running a C++ pokerbot.
*/
int main(int argc, char *argv[]) {
  auto [host, port] = parseArgs(argc, argv);
  runBot<Bot>(host, port);
  return 0;
}
