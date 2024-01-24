#include "round.h"
#include <fmt/format.h>
#include <stdexcept>

namespace pokerbot::round {

const Round& Round::prev_round() const {
  if (id == 0) {
    throw std::invalid_argument(fmt::format("Can't call prev_round() on {}", to_string()));
  }
  return ROUNDS[id - 1];
}

const Round& Round::next_round() const {
  if (id == ROUNDS.size() - 1) {
    throw std::invalid_argument(fmt::format("Can't call next_round() on {}", to_string()));
  }
  return ROUNDS[id + 1];
}

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

const Round& from_id(unsigned round_id) {
  for (const auto& r : ROUNDS) {
    if (r.id == round_id) {
      return r;
    }
  }
  throw std::invalid_argument(
      fmt::format("round_from_id: Invalid round_id provided: {}", round_id));
}

const Round& from_name(const std::string& name) {
  for (const auto& r : ROUNDS) {
    if (r.to_string() == name) {
      return r;
    }
  }
  throw std::invalid_argument(fmt::format("round_from_name: Invalid name provided: {}", name));
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

const Round& from_board_str(const std::string& board_str) {
  for (const auto& r : ROUNDS) {
    if (2 * r.num_board_cards == board_str.size()) {
      return r;
    }
  }
  throw std::invalid_argument(
      fmt::format("round_from_board: Invalid board_str provided: {}", board_str));
}

}  // namespace pokerbot::round