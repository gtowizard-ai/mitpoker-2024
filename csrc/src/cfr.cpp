#include "cfr.h"

#include <chrono>

#include "equity.h"
#include "preflop_equity.h"
#include "states.h"

namespace pokerbot {

CFR::CFR(const Game& game) : game_(game), board_data_cache_(game) {
  for (auto& child_values : children_cfvs_) {
    child_values.resize(ceil_to_multiple(NUM_HANDS_POSTFLOP_3CARDS));
  }
}

void CFR::build_tree() {
  actions_.clear();
  actions_.reserve(legal_actions().size() + 1);

  const auto legal_actions = root_->legal_actions();

  bool raise_is_legal = false;
  for (const auto legal_action : legal_actions) {
    if (legal_action != Action::Type::RAISE) {
      actions_.emplace_back(legal_action);
    } else {
      raise_is_legal = true;
    }
  }

  // NB: We need to put the raise option as the last one since some functions depend on it
  if (raise_is_legal) {
    // Prefer to go all-in when it's less than X * pot-sized-bet
    // Otherwise always choose a pot-sized-bet
    const auto raise_bounds = root_->raise_bounds();
    auto amount_call = std::max(root_->bets[0], root_->bets[1]);
    auto pot_plus_call = root_->pot() + std::abs(root_->bets[1] - root_->bets[0]);
    auto pot_sized_bet = amount_call + pot_plus_call;

    if (raise_bounds[1] < 3 * pot_sized_bet) {
      // All-in
      actions_.emplace_back(Action::Type::RAISE, raise_bounds[1]);
    } else {
      actions_.emplace_back(Action::Type::RAISE, pot_sized_bet);
    }
  }

  if (actions_.size() > MAX_LEGAL_ACTIONS) {
    throw std::runtime_error("Tree too big");
  }
}

void CFR::compute_fold_cfvs(const Range& traverser_range, const Range& opponent_range, float payoff,
                            std::vector<float>& cfvs) const {
  std::fill_n(cfvs.begin(), traverser_range.num_hands(), 0);
  compute_cfvs_fixed_payoff(game_, traverser_range, opponent_range, root_->board_cards, cfvs,
                            payoff, board_data_cache_);
}

void CFR::compute_showdown_cfvs(const Range& traverser_range, const Range& opponent_range,
                                float payoff, std::vector<float>& cfvs) const {
  std::fill_n(cfvs.begin(), traverser_range.num_hands(), 0);
  if (root_->round() == round::PREFLOP) {
    // FIXME - Does this make sense?
    if (payoff >= STARTING_STACK) {
      compute_cfvs_preflop(opponent_range, payoff, cfvs, PREFLOP_3_CARDS_PAYOFFS);
    } else {
      compute_cfvs_preflop(opponent_range, payoff, cfvs, PREFLOP_2_CARDS_PAYOFFS);
    }
  } else {
    compute_cfvs_showdown<float>(game_, traverser_range, opponent_range, root_->board_cards, cfvs,
                                 payoff, board_data_cache_);
  }
}

void CFR::update_opponent_cfvs_vs_bet() {
  if (actions_.back().type != Action::Type::RAISE) {
    return;
  }
  const auto& raise_action = actions_.back();

  // Opponent's perspective -> Fold means he lose
  const float half_pot = root_->pot_start_round() * 0.5f;
  const float fold_payoff = -(half_pot + root_->bets[1 - player_id_]);
  compute_fold_cfvs(opponent_range_raise_fold_, hero_range_raise_, fold_payoff, raise_fold_cfvs_);

  // Call
  const float call_payoff = half_pot + raise_action.amount;
  compute_showdown_cfvs(opponent_range_raise_call_, hero_range_raise_, call_payoff,
                        raise_call_cfvs_);
}

void CFR::update_hero_cfvs_bet_node() {
  if (actions_.back().type != Action::Type::RAISE) {
    return;
  }
  const auto& raise_action = actions_.back();

  // Hero's perspective -> Opponent folds mean we wins
  const float half_pot = root_->pot_start_round() * 0.5f;
  const float fold_payoff = half_pot + root_->bets[1 - player_id_];
  compute_fold_cfvs(hero_range_raise_, opponent_range_raise_fold_, fold_payoff, raise_fold_cfvs_);

  // Call
  const float call_payoff = half_pot + raise_action.amount;
  compute_showdown_cfvs(hero_range_raise_, opponent_range_raise_call_, call_payoff,
                        raise_call_cfvs_);

  auto& cfvs = children_cfvs_[num_actions() - 1];
  for (hand_t hand = 0; hand < hero_range_raise_.num_hands(); ++hand) {
    cfvs[hand] = raise_fold_cfvs_[hand] + raise_call_cfvs_[hand];
  }
}

void CFR::precompute_cfvs_fixed_nodes(const std::array<Range, 2>& ranges) {
  for (auto& child_values : children_cfvs_) {
    std::fill_n(child_values.begin(), ranges[player_id_].num_hands(), 0);
  }

  for (unsigned a = 0; a < num_actions(); ++a) {
    if (actions_[a].type == Action::Type::RAISE) {
      // For raise nodes, we let our opponent choose between call/fold
      // so we can't precompute the values
      continue;
    }

    const float half_pot = root_->pot_start_round() * 0.5f;
    if (actions_[a].type == Action::Type::FOLD) {
      const float payoff = -(half_pot + root_->bets[player_id_]);
      compute_fold_cfvs(ranges[player_id_], ranges[1 - player_id_], payoff, children_cfvs_[a]);
    } else {
      // Call or Check
      const float payoff = half_pot + root_->bets[1 - player_id_];
      compute_showdown_cfvs(ranges[player_id_], ranges[1 - player_id_], payoff, children_cfvs_[a]);
    }
  }
}

void CFR::update_opponent_regrets() {
  const auto regret_discount = get_linear_cfr_discount_factor();
  for (hand_t hand = 0; hand < num_hands_[1 - player_id_]; ++hand) {
    const float root_value = opponent_fold_strategy_vs_bet_[hand] * raise_fold_cfvs_[hand] +
                             (1.0f - opponent_fold_strategy_vs_bet_[hand]) * raise_call_cfvs_[hand];

    opponent_regrets_vs_bet_(0, hand) += raise_fold_cfvs_[hand] - root_value;
    opponent_regrets_vs_bet_(1, hand) += raise_call_cfvs_[hand] - root_value;

    opponent_regrets_vs_bet_(0, hand) *= regret_discount;
    opponent_regrets_vs_bet_(1, hand) *= regret_discount;
  }
}

void CFR::update_hero_regrets() {
  const auto regret_discount = get_linear_cfr_discount_factor();

  static_assert(RANGE_SIZE_MULTIPLE % 16 == 0);
  for (hand_t hand = 0; hand < num_hands_[player_id_]; hand += 16) {
    std::array<float, 16> root_values = {};

    for (unsigned action = 0; action < num_actions(); ++action) {
      for (unsigned i = 0; i < 16; ++i) {
        root_values[i] += strategy_(action, hand + i) * children_cfvs_[action][hand + i];
      }
    }

    for (unsigned action = 0; action < num_actions(); ++action) {
      for (unsigned i = 0; i < 16; ++i) {
        const auto immediate_regret = children_cfvs_[action][hand + i] - root_values[i];
        regrets_(action, hand + i) += immediate_regret;
        regrets_(action, hand + i) *= regret_discount;
      }
    }
  }
}

void CFR::update_hero_reaches(const Range& hero_range) {
  const auto& raise_action = actions_.back();
  if (raise_action.type != Action::Type::RAISE) {
    return;
  }

  const auto raise_action_index = num_actions() - 1;
  for (hand_t hand = 0; hand < num_hands_[player_id_]; ++hand) {
    const auto raise_prob = strategy_(raise_action_index, hand);
    hero_range_raise_.range[hand] = hero_range.range[hand] * raise_prob;
  }
}

void CFR::update_opponent_reaches(const Range& opponent_range) {
  for (hand_t hand = 0; hand < num_hands_[1 - player_id_]; ++hand) {
    const auto fold_prob = opponent_fold_strategy_vs_bet_[hand];
    opponent_range_raise_fold_.range[hand] = opponent_range.range[hand] * fold_prob;
    opponent_range_raise_call_.range[hand] = opponent_range.range[hand] * (1.0f - fold_prob);
  }
}

void CFR::update_hero_strategy() {
  static_assert(RANGE_SIZE_MULTIPLE % 16 == 0);
  for (hand_t hand = 0; hand < num_hands_[player_id_]; hand += 16) {
    std::array<float, 16> sum_positive_regrets = {};

    for (unsigned action = 0; action < num_actions(); ++action) {
      for (unsigned i = 0; i < 16; ++i) {
        const float positive_regret = std::max(regrets_(action, hand + i), 0.0f);
        sum_positive_regrets[i] += positive_regret;
        strategy_(action, hand + i) = positive_regret;
      }
    }

    for (unsigned action = 0; action < num_actions(); ++action) {
      for (unsigned i = 0; i < 16; ++i) {
        if (sum_positive_regrets[i] > 0) {
          strategy_(action, hand + i) /= sum_positive_regrets[i];
        } else {
          strategy_(action, hand + i) = 1.0f / static_cast<float>(num_actions());
        }
      }
    }
  }

  // zero out strategy for hands outside of range just in case
  for (hand_t hand = num_hands_[player_id_]; hand < ceil_to_multiple(num_hands_[player_id_], 16);
       ++hand) {
    for (unsigned action = 0; action < num_actions(); ++action) {
      strategy_(action, hand) = 0;
    }
  }
}

void CFR::update_opponent_strategy() {
  static_assert(RANGE_SIZE_MULTIPLE % 16 == 0);
  for (hand_t hand = 0; hand < num_hands_[1 - player_id_]; hand += 16) {
    std::array<float, 16> sum_positive_regrets = {};

    // Fold
    for (unsigned i = 0; i < 16; ++i) {
      const float positive_regret = std::max(opponent_regrets_vs_bet_(0, hand + i), 0.0f);
      sum_positive_regrets[i] += positive_regret;
      opponent_fold_strategy_vs_bet_[hand + i] = positive_regret;
    }

    // Call
    for (unsigned i = 0; i < 16; ++i) {
      const float positive_regret = std::max(opponent_regrets_vs_bet_(1, hand + i), 0.0f);
      sum_positive_regrets[i] += positive_regret;
    }

    for (unsigned i = 0; i < 16; ++i) {
      if (sum_positive_regrets[i] > 0) {
        opponent_fold_strategy_vs_bet_[hand + i] /= sum_positive_regrets[i];
      } else {
        opponent_fold_strategy_vs_bet_[hand + i] = 0.5f;
      }
    }
  }
}

float CFR::get_linear_cfr_discount_factor() const {
  // We always have uniform strategy, hence +1.
  const auto num_iterations = static_cast<float>(num_steps_ + 1);
  return num_iterations / (num_iterations + 1);
}

void CFR::step(const std::array<Range, 2>& ranges) {
  update_hero_cfvs_bet_node();
  update_hero_regrets();
  update_hero_strategy();
  update_hero_reaches(ranges[player_id_]);

  update_opponent_cfvs_vs_bet();
  update_opponent_regrets();
  update_opponent_strategy();
  update_opponent_reaches(ranges[1 - player_id_]);

  num_steps_++;
}

void CFR::solve(const std::array<Range, 2>& ranges, const RoundStatePtr& state,
                const unsigned player_id, const float time_budget_ms,
                const unsigned max_num_iters) {
  const auto start_time = std::chrono::high_resolution_clock::now();
  root_ = state;
  num_hands_ = {
      ranges[0].num_hands(),
      ranges[1].num_hands(),
  };
  const auto max_num_hands = std::max(ranges[0].num_hands(), ranges[1].num_hands());
  player_id_ = player_id;
  num_steps_ = 0;

  hero_range_raise_ = ranges[player_id_];
  opponent_range_raise_call_ = ranges[1 - player_id_];
  opponent_range_raise_fold_ = ranges[1 - player_id_];

  build_tree();

  regrets_ = HandActionsValues(num_actions(), num_hands_[player_id_], 0);
  opponent_regrets_vs_bet_ = HandActionsValues(2, num_hands_[1 - player_id_], 0);
  strategy_ = HandActionsValues(num_actions(), num_hands_[player_id_], 1.0f / num_actions());
  opponent_fold_strategy_vs_bet_.assign(ceil_to_multiple(num_hands_[1 - player_id_]), 0.5f);
  raise_fold_cfvs_.assign(ceil_to_multiple(max_num_hands), 0);
  raise_call_cfvs_.assign(ceil_to_multiple(max_num_hands), 0);

  precompute_cfvs_fixed_nodes(ranges);
  update_hero_reaches(ranges[player_id_]);
  update_opponent_reaches(ranges[1 - player_id_]);

  while (true) {
    step(ranges);

    const auto duration_micros = std::chrono::duration_cast<std::chrono::microseconds>(
                                     std::chrono::high_resolution_clock::now() - start_time)
                                     .count();
    if (static_cast<float>(duration_micros) >= 1000 * time_budget_ms) {
      fmt::print("Stopping CFR after {} iters (Reached time budget) \n", num_steps_);
      break;
    }
    if (num_steps_ >= max_num_iters) {
      fmt::print("Stopping CFR after {} iters (Reached max number of iters of {}) \n", num_steps_,
                 max_num_iters);
      break;
    }
  }
}

}  // namespace pokerbot
