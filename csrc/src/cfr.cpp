#include "cfr.h"
#include "equity.h"
#include "poker_hand.h"
#include "preflop_equity.h"
#include "ranges_utils.h"
#include "states.h"

namespace pokerbot {

HandActionsValues::HandActionsValues(const unsigned num_hands, const unsigned num_actions,
                                     float value)
    : data(num_hands * num_actions, value), num_hands_(num_hands), num_actions_(num_actions) {}

CFR::CFR(const Game& game) : game_(game) {
  for (auto& child_values : children_values_) {
    child_values.resize(NUM_HANDS_POSTFLOP_3CARDS);
  }
}

void CFR::build_tree(const RoundStatePtr& state) {
  actions_.clear();
  actions_.reserve(legal_actions().size() + 1);

  const auto legal_actions = state->legal_actions();

  bool raise_avail = false;
  for (const auto legal_action : legal_actions) {
    if (legal_action != Action::Type::RAISE) {
      actions_.emplace_back(legal_action);
    } else {
      raise_avail = true;
    }
  }

  // NB: PUT RAISE ACTION AS LAST ONE
  if (raise_avail) {
    // Add pot-sized bet if it's less than going all-in by `pot`
    const auto raise_bounds = state->raise_bounds();
    auto pot_sized_bet = std::max(raise_bounds[0], state->pot());
    if (raise_bounds[1] - pot_sized_bet > state->pot()) {
      actions_.emplace_back(Action::Type::RAISE, pot_sized_bet);
    } else {
      // All-in
      actions_.emplace_back(Action::Type::RAISE, raise_bounds[1]);
    }
  }

  if (actions_.size() > max_available_actions_) {
    throw std::runtime_error("Tree too big");
  }
}

void CFR::update_opponent_cfvs_vs_bet() {
  if (actions_.back().type != Action::Type::RAISE) {
    return;
  }
  const auto& raise_action = actions_.back();

  // FIXME PAYOFFS -> Well defined?

  // Opponent is the one folding
  Payoff fold_payoff;
  fold_payoff.win = -((root_->pot() / 2) + root_->bets[1 - player_id_]);
  fold_payoff.lose = fold_payoff.win;

  std::fill_n(raise_fold_cfvs_.begin(), opponent_range_raise_fold_.num_hands(), 0);
  compute_cfvs_river<float>(game_, opponent_range_raise_fold_, hero_range_raise_,
                            PokerHand(root_->board_cards), raise_fold_cfvs_, fold_payoff);

  // CALL
  Payoff call_payoff;
  call_payoff.win = (root_->pot() / 2) + raise_action.amount;
  call_payoff.lose = -call_payoff.win;

  std::fill_n(raise_call_cfvs_.begin(), opponent_range_raise_call_.num_hands(), 0);
  compute_cfvs_river<float>(game_, opponent_range_raise_call_, hero_range_raise_,
                            PokerHand(root_->board_cards), raise_call_cfvs_, call_payoff);
}

void CFR::update_hero_cfvs_bet_node() {
  if (actions_.back().type != Action::Type::RAISE) {
    return;
  }
  const auto& raise_action = actions_.back();

  // FIXME PAYOFFS -> Well defined?

  // Opponent is the one folding so we win
  Payoff fold_payoff;
  fold_payoff.win = (root_->pot() / 2) + root_->bets[1 - player_id_];
  fold_payoff.lose = fold_payoff.win;

  // FIXME -> HANDLE ALL CASES (Preflop/flop/turn/river)

  std::fill_n(raise_fold_cfvs_.begin(), hero_range_raise_.num_hands(), 0);
  compute_cfvs_river<float>(game_, hero_range_raise_, opponent_range_raise_fold_,
                            PokerHand(root_->board_cards), raise_fold_cfvs_, fold_payoff);

  // CALL
  Payoff call_payoff;
  call_payoff.win = (root_->pot() / 2) + raise_action.amount;
  call_payoff.lose = -call_payoff.win;

  std::fill_n(raise_call_cfvs_.begin(), hero_range_raise_.num_hands(), 0);
  compute_cfvs_river<float>(game_, hero_range_raise_, opponent_range_raise_call_,
                            PokerHand(root_->board_cards), raise_call_cfvs_, call_payoff);

  auto& cfvs = children_values_[num_actions() - 1];
  for (hand_t hand = 0; hand < hero_range_raise_.num_hands(); ++hand) {
    cfvs[hand] = raise_fold_cfvs_[hand] + raise_call_cfvs_[hand];
  }
}

void CFR::precompute_cfvs_fixed_nodes(const std::array<Range, 2>& ranges,
                                      const RoundStatePtr& state) {
  for (auto& child_values : children_values_) {
    std::fill_n(child_values.begin(), ranges[player_id_].num_hands(), 0);
  }

  for (unsigned a = 0; a < num_actions(); ++a) {
    if (actions_[a].type == Action::Type::RAISE) {
      continue;
    }

    const Payoff payoff = [&] {
      if (actions_[a].type == Action::Type::FOLD) {
        Payoff fold_payoff;
        fold_payoff.win = -(root_->pot() / 2 + root_->bets[player_id_]);
        fold_payoff.lose = fold_payoff.win;
        return fold_payoff;
      }
      // Call or Check

      Payoff call_payoff;
      call_payoff.win = (root_->pot() / 2) + ranges::sum(root_->bets);
      call_payoff.lose = -call_payoff.win;
      return call_payoff;
    }();

    // FIXME -> HANDLE ALL CASES
    if (state->round() == round::PREFLOP) {
      compute_cfvs_preflop(ranges[1 - player_id_], payoff, children_values_[a]);
    } else {
      compute_cfvs_river<float>(game_, ranges[player_id_], ranges[1 - player_id_],
                                PokerHand(state->board_cards), children_values_[a], payoff, false);
    }
  }
}

void CFR::update_opponent_regrets() {
  const auto regret_discount = get_linear_cfr_discount_factor();
  for (hand_t hand = 0; hand < num_hands_[1 - player_id_]; ++hand) {
    const float root_value = opponent_fold_strategy_vs_bet_[hand] * raise_fold_cfvs_[hand] +
                             (1.0f - opponent_fold_strategy_vs_bet_[hand]) * raise_call_cfvs_[hand];

    opponent_regrets_vs_bet_(hand, 0) += raise_fold_cfvs_[hand] - root_value;
    opponent_regrets_vs_bet_(hand, 1) += raise_call_cfvs_[hand] - root_value;

    opponent_regrets_vs_bet_(hand, 0) *= regret_discount;
    opponent_regrets_vs_bet_(hand, 1) *= regret_discount;
  }
}

void CFR::update_hero_regrets() {
  const auto regret_discount = get_linear_cfr_discount_factor();
  for (hand_t hand = 0; hand < num_hands_[player_id_]; ++hand) {
    float root_value = 0;
    for (unsigned action = 0; action < num_actions(); action++) {
      root_value += strategy_(hand, action) * children_values_[action][hand];
    }

    for (unsigned action = 0; action < num_actions(); action++) {
      const auto immediate_regret = children_values_[action][hand] - root_value;
      regrets_(hand, action) += immediate_regret;
      regrets_(hand, action) *= regret_discount;
    }
  }
}

void CFR::update_hero_reaches(const Range& hero_range) {
  const auto& raise_action = actions_.back();
  if (raise_action.type != Action::Type::RAISE) {
    throw std::runtime_error("Not implemented");
    return;
  }

  for (hand_t hand = 0; hand < num_hands_[player_id_]; ++hand) {
    const auto raise_prob = strategy_(hand, num_actions() - 1);
    hero_range_raise_.range[hand] = hero_range.range[hand] * raise_prob;
  }
}

void CFR::update_opponent_reaches(const Range& opponent_range) {
  for (hand_t hand = 0; hand < num_hands_[1 - player_id_]; ++hand) {
    const auto fold_prob = opponent_fold_strategy_vs_bet_[hand];
    opponent_range_raise_fold_.range[hand] = opponent_range.range[hand] * fold_prob;
    opponent_range_raise_call_.range[hand] = opponent_range.range[hand] * (1.0f - fold_prob);
  }

  // for (hand_t hand = 0; hand < num_hands_[1 - player_id_]; ++hand) {
  //   if (!std::isfinite(opponent_range_raise_fold_.range[hand]) || !std::isfinite(opponent_range_raise_call_.range[hand])) {
  //     throw std::runtime_error(
  //         fmt::format("Not finite opponent_range_raise_ / range = {} / strat = {} / regret = {} / fold cfv = {} / call cfv = {}",
  //                     opponent_range.range[hand], opponent_strategy_vs_bet_(hand, 0), opponent_regrets_vs_bet_(hand, 0),
  //                     raise_fold_cfvs_[hand], raise_call_cfvs_[hand]));
  //   }
  // }
}

void CFR::update_hero_strategy() {
  for (unsigned hand = 0; hand < num_hands_[player_id_]; hand++) {
    float sum_positive_regrets = 0;
    for (unsigned action = 0; action < num_actions(); action++) {
      if (regrets_(hand, action) > 0) {
        sum_positive_regrets += regrets_(hand, action);
        strategy_(hand, action) = regrets_(hand, action);
      } else {
        strategy_(hand, action) = 0;
      }
    }

    for (unsigned action = 0; action < num_actions(); action++) {
      strategy_(hand, action) = sum_positive_regrets > 0
                                    ? strategy_(hand, action) / sum_positive_regrets
                                    : 1.0f / static_cast<float>(num_actions());
    }
  }
}

void CFR::update_opponent_strategy() {
  for (unsigned hand = 0; hand < num_hands_[1 - player_id_]; hand++) {
    float sum_positive_regrets = 0;

    // Fold
    if (opponent_regrets_vs_bet_(hand, 0) > 0) {
      sum_positive_regrets += opponent_regrets_vs_bet_(hand, 0);
      opponent_fold_strategy_vs_bet_[hand] = opponent_regrets_vs_bet_(hand, 0);
    } else {
      opponent_fold_strategy_vs_bet_[hand] = 0;
    }

    // Call
    if (opponent_regrets_vs_bet_(hand, 1) > 0) {
      sum_positive_regrets += opponent_regrets_vs_bet_(hand, 1);
    }

    opponent_fold_strategy_vs_bet_[hand] =
        sum_positive_regrets > 0 ? opponent_fold_strategy_vs_bet_[hand] / sum_positive_regrets
                                 : 0.5f;
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

HandActionsValues CFR::solve(const std::array<Range, 2>& ranges, const RoundStatePtr& state,
                             const unsigned player_id, const float time_budget_ms) {
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

  build_tree(state);

  regrets_ = HandActionsValues(num_hands_[player_id_], num_actions(), 0);
  opponent_regrets_vs_bet_ = HandActionsValues(num_hands_[1 - player_id_], 2, 0);
  strategy_ = HandActionsValues(num_hands_[player_id_], num_actions(), 1.0f / num_actions());
  opponent_fold_strategy_vs_bet_.assign(num_hands_[1 - player_id_], 0.5f);
  raise_fold_cfvs_.assign(max_num_hands, 0);
  raise_call_cfvs_.assign(max_num_hands, 0);

  precompute_cfvs_fixed_nodes(ranges, state);
  update_hero_reaches(ranges[player_id_]);
  update_opponent_reaches(ranges[1 - player_id_]);

  const unsigned max_iterations = 10;  // FIXME
  for (unsigned i = 0; i < max_iterations; ++i) {
    step(ranges);
  }
  fmt::print("Completed {} CFR iterations \n", max_iterations);

  return strategy_;
}

}  // namespace pokerbot
