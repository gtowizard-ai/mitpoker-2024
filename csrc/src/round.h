#pragma once
#include <array>
#include <string>

namespace gtowizard::round {

struct Round {
  Round(unsigned id, unsigned num_board_cards) : id(id), num_board_cards(num_board_cards) {}

  std::string to_string() const;

  bool operator==(const Round& rhs) const { return id == rhs.id; }
  bool operator!=(const Round& rhs) const { return !(*this == rhs); }
  auto operator<(const Round& rhs) const { return id < rhs.id; }
  auto operator>(const Round& rhs) const { return id > rhs.id; }

  unsigned id;
  unsigned num_board_cards;
};

const Round PREFLOP(0, 0);
const Round FLOP(1, 3);
const Round TURN(2, 4);
const Round RIVER(3, 5);

const std::array<Round, 4> ROUNDS = {PREFLOP, FLOP, TURN, RIVER};

const Round& from_num_cards(unsigned num_board_cards);

}  // namespace gtowizard::round