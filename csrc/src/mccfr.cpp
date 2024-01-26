#include "mccfr.h"
#include <chrono>
#include "equity.h"
#include "poker_hand.h"
#include "preflop_equity.h"
#include "states.h"

namespace pokerbot {

HandActionsValues::HandActionsValues(const unsigned num_hands, const unsigned num_actions)
    : data(num_hands * num_actions, 0), num_hands_(num_hands), num_actions_(num_actions) {}

// ToDo: Using rgn can be time consuming. Consider using faster/less accurate methods
// ToDo: Unify the sampling functions
unsigned sample_index(const std::vector<float>& distribution_values, const unsigned start_point,
                      const unsigned length, std::minstd_rand& gen) {
  std::discrete_distribution<unsigned> distribution(
      distribution_values.begin() + start_point,
      distribution_values.begin() + start_point + length);
  return distribution(gen);
}

unsigned sample_index(const std::array<float, NUM_HANDS_POSTFLOP_3CARDS>& distribution_values,
                      const unsigned start_point, const unsigned length, std::minstd_rand& gen) {
  std::discrete_distribution<unsigned> distribution(
      distribution_values.begin() + start_point,
      distribution_values.begin() + start_point + length);
  return distribution(gen);
}

unsigned sample_index(const unsigned length, std::minstd_rand& gen) {
  std::uniform_int_distribution<unsigned> distribution(0, length);
  return distribution(gen);
}

MCCFR::MCCFR(const Game& game, const unsigned warm_up_iterations)
    : game_(game),
      random_generator_(std::random_device()()),
      warm_up_iterations_(warm_up_iterations),
      regrets_(NUM_HANDS_POSTFLOP_3CARDS, max_available_actions_) {
  for (auto& child_values : children_values_) {
    child_values.resize(NUM_HANDS_POSTFLOP_3CARDS);
  }
}

void MCCFR::build_tree(const RoundStatePtr& state) {
  available_actions_.clear();
  available_actions_.reserve(legal_actions().size() + 1);

  const auto legal_actions = state->legal_actions();
  const auto raise_bounds = state->raise_bounds();

  for (const auto legal_action : legal_actions) {
    if (legal_action != Action::Type::RAISE) {
      available_actions_.emplace_back(legal_action);
    } else {
      // Add pot-sized bet if it's less than going all-in by `pot`
      auto pot_sized_bet = std::max(raise_bounds[0], state->pot());
      if (raise_bounds[1] - pot_sized_bet < state->pot()) {
        available_actions_.emplace_back(Action::Type::RAISE, pot_sized_bet);
      }
      // All-in
      available_actions_.emplace_back(Action::Type::RAISE, raise_bounds[1]);
    }
  }
}

void MCCFR::precompute_child_values(const std::array<Range, 2>& ranges,
                                    const RoundStatePtr& state) {

  const float opp_contribution = STARTING_STACK - state->stacks[1 - player_id_];
  const float my_contribution = STARTING_STACK - state->stacks[player_id_];

  for (auto& child_values : children_values_) {
    std::fill_n(child_values.begin(), ranges[player_id_].num_hands(), 0);
  }

  for (unsigned action = 0; action < num_actions(); action++) {
    const Payoff payoff = [&] {
      if (available_actions_[action].action_type == Action::Type::FOLD) {
        return Payoff{
            -my_contribution,
            0,
            -my_contribution,
        };
      }
      const float next_round_check = available_actions_[action].action_type == Action::Type::RAISE
                                         ? available_actions_[action].amount
                                         : 0.0f;
      return Payoff{
          opp_contribution + next_round_check,
          my_contribution - ((opp_contribution + my_contribution) / 2),
          -my_contribution - next_round_check,
      };
    }();

    if (state->round() == round::PREFLOP) {
      // TODO
      compute_cfvs_preflop(ranges[1 - player_id_], payoff, children_values_[action]);
    } else {
      compute_cfvs_river<float>(game_, ranges[player_id_], ranges[1 - player_id_],
                                PokerHand(state->board_cards), children_values_[action], payoff,
                                false);
    }
  }
}

float MCCFR::get_child_value(const unsigned hand, const unsigned action) const {
  if (available_actions_[action].action_type == Action::Type::FOLD) {
    return -static_cast<float>(my_contribution_);
  } else {
    return static_cast<float>(children_values_[action][hand]);
  }
}

void MCCFR::update_root_value(const unsigned hand) {
  double root_value = 0;

  double normalization = 0;
  for (unsigned action = 0; action < num_actions(); action++) {
    normalization += regrets_(hand, action);
  }

  for (unsigned action = 0; action < num_actions(); action++) {
    root_value += (normalization > 0 ? regrets_(hand, action) / normalization
                                     : 1.0f / static_cast<float>(num_actions())) *
                  get_child_value(hand, action);
  }
  values_[hand] = static_cast<float>(root_value);
}

void MCCFR::update_root_value() {
  for (unsigned hand = 0; hand < num_hands_; hand++) {
    update_root_value(hand);
  }
}

void MCCFR::update_regrets(const std::array<Range, 2>& ranges) {
  // ToDo: Add epsilon-greedy selection?

  // sample a hand
  const unsigned hand =
      sample_index(ranges[player_id_].range, 0, ranges[player_id_].num_hands(), random_generator_);

  // sample an action - if there is warm-up state sample uniformely
  const unsigned action = [&] {
    if (num_steps_[hand] < warm_up_iterations_) {
      return sample_index(regrets_.data, hand * max_available_actions_, available_actions_.size(),
                          random_generator_);
    } else {
      return sample_index(num_actions(), random_generator_);
    }
  }();

  const auto action_value = get_child_value(hand, action);

  update_root_value(hand);

  // Update regrets
  const float diff = action_value - values_[hand];
  if (diff > 0) {
    regrets_(hand, action) += diff * get_linear_cfr_discount_factor(hand);
  }

  ++num_steps_[hand];
}

HandActionsValues MCCFR::get_last_strategy() {
  HandActionsValues strategy(num_hands_, num_actions());
  for (unsigned hand = 0; hand < num_hands_; hand++) {

    double normalization = 0;
    for (unsigned action = 0; action < num_actions(); action++) {
      normalization += regrets_(hand, action);
    }

    for (unsigned action = 0; action < num_actions(); action++) {
      strategy(hand, action) = normalization > 0
                                   ? regrets_(hand, action) / static_cast<float>(normalization)
                                   : 1.0f / static_cast<float>(num_actions());
    }
  }
  return strategy;
}

float MCCFR::get_linear_cfr_discount_factor(const unsigned hand) const {
  // We always have uniform strategy, hence +1.
  const auto num_iterations = static_cast<float>(num_steps_[hand] + 1);

  return num_iterations / (num_iterations + 1);
}

void MCCFR::initial_regrets() {
  // Iterate over all hands and actions once to prevent biased selection of actions in sampling
  update_root_value();
  for (unsigned hand = 0; hand < num_hands_; hand++) {
    for (unsigned action = 0; action < num_actions(); action++) {
      const auto action_value = get_child_value(hand, action);
      // Update regrets
      const float diff = action_value - values_[hand];
      if (diff > 0) {
        regrets_(hand, action) += diff;
      }
    }
  }
}

HandActionsValues MCCFR::solve(const std::array<Range, 2>& ranges, const RoundStatePtr& state,
                               const unsigned player_id, const float time_budget_ms) {
  const auto timer_function_start = std::chrono::high_resolution_clock::now();

  // Initialize variable for this solve
  num_hands_ = ranges[player_id].num_hands();
  player_id_ = player_id;

  my_contribution_ = STARTING_STACK - state->stacks[player_id_];

  std::fill_n(values_.begin(), num_hands_, 0);
  std::fill(regrets_.data.begin(), regrets_.data.end(), 0);
  std::fill_n(num_steps_.begin(), num_hands_, 0);

  build_tree(state);

  precompute_child_values(ranges, state);

  // estimate how much it would take to go through all hands and actions
  initial_regrets();

  const auto timer_before_mccfr = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < time_checkpoints_; i++) {
    // Compute cumulative regrets and counterfactual values.
    // and generate strategy profile from the regrets
    update_regrets(ranges);
  }

  const auto timer_after_checkpoint = std::chrono::high_resolution_clock::now();

  const auto initialization_passed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                              timer_before_mccfr - timer_function_start)
                                              .count();
  const auto mccfr_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                              timer_after_checkpoint - timer_before_mccfr)
                              .count();
  const auto max_iterations =
      static_cast<long long>((time_budget_ms - initialization_passed_time) * timer_error_bound_ *
                             time_checkpoints_ / mccfr_time);

  for (unsigned i = 0; i < max_iterations; ++i) {
    // Compute cumulative regrets and counterfactual values.
    // and generate strategy profile from the regrets
    update_regrets(ranges);
  }
  fmt::print("Completed {} MCCFR iterations \n", max_iterations);

  return get_last_strategy();
}

}  // namespace pokerbot
