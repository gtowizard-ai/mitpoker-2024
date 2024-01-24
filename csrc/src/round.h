#pragma once
#include <array>
#include <string>

namespace pokerbot::round {

struct Round {
  Round(unsigned id, unsigned num_board_cards, unsigned num_board_cards_deal)
      : id(id), num_board_cards(num_board_cards), num_board_cards_deal(num_board_cards_deal) {}

  std::string to_string() const;

  bool operator==(const Round& rhs) const { return id == rhs.id; }
  bool operator!=(const Round& rhs) const { return !(*this == rhs); }
  auto operator<(const Round& rhs) const { return id < rhs.id; }
  auto operator>(const Round& rhs) const { return id > rhs.id; }

  // Get previous round. Throws error if round is the first one
  const Round& prev_round() const;

  // Get next round. Throws error if round is already the last one
  const Round& next_round() const;

  unsigned id;
  unsigned num_board_cards;
  unsigned num_board_cards_deal;
};

const Round PREFLOP(0, 0, 0);
const Round FLOP(1, 3, 3);
const Round TURN(2, 4, 1);
const Round RIVER(3, 5, 1);

const std::array<Round, 4> ROUNDS = {PREFLOP, FLOP, TURN, RIVER};

const Round& from_id(unsigned id);
const Round& from_name(const std::string& name);
const Round& from_num_cards(unsigned num_board_cards);
// Infer round from board string like "AcAdKh"
const Round& from_board(const std::string& board_str);

}  // namespace pokerbot::round