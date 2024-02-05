#include "round.h"
#include <fmt/format.h>
#include <stdexcept>

namespace gtowizard::round {

std::string Round::to_string() const {
  switch (id) {
    case 0:
      return "preflop";
    case 1:
      return "flop";
    case 2:
      return "turn";
    case 3:
      return "river";
    default:
      throw std::runtime_error("In unknown round!");
  }
}

const Round& from_num_cards(unsigned num_board_cards) {
  for (const auto& r : ROUNDS) {
    if (r.num_board_cards == num_board_cards) {
      return r;
    }
  }
  throw std::invalid_argument(
      fmt::format("round_from_num_cards: Invalid num_board_cards provided: {}", num_board_cards));
}

}  // namespace gtowizard::round