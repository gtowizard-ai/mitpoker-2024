#include "../src/main_bot.h"
#include "../src/runner.h"
using namespace pokerbot;

/*
  Main program for running our pokerbot
*/
int main(int argc, char* argv[]) {
  auto [host, port] = parseArgs(argc, argv);
  runBot<MainBot>(host, port);
  return 0;
}
