#include "mccfr.h"
#include "ranges_utils.h"

namespace pokerbot {

HandActionsValues::HandActionsValues(const unsigned num_hands, const unsigned num_actions)
    : num_hands_(num_hands) {
  data.reserve(num_hands_ * num_actions);
  std::fill(data.begin(), data.end(), 0);
}

// ToDo: Using rgn can be time consuming. Consider using faster/less accurate methods
unsigned sample_index(const std::vector<float>& distribution_values, const unsigned start_point,
                      const unsigned length, std::mt19937 gen) {
  std::discrete_distribution<unsigned> distribution(
      distribution_values.begin() + start_point,
      distribution_values.begin() + start_point + length);

  return distribution(gen);
}

unsigned sample_index(const unsigned length, std::mt19937 gen) {
  std::uniform_int_distribution<unsigned> distribution(
        0, length);

  return distribution(gen);
}

unsigned sample_index(const std::array<float, NUM_HANDS_POSTFLOP_3CARDS>& distribution_values,
                      std::mt19937 gen) {
  std::discrete_distribution<unsigned> distribution(distribution_values.begin(),
                                                    distribution_values.end());

  return distribution(gen);
}

MCCFR::MCCFR(const GameInfo& game_state, const RoundStatePtr& round_state, const unsigned player_id,
             const std::vector<Range>& ranges, const unsigned warm_up_iterations)
    : game_(game_state),
      round_state_(round_state_),
      player_(player_id),
      ranges_(ranges),
      num_hands_(ranges[player_id].range.size()),
      values_(num_hands_, 0),
      random_generator_(std::random_device()()),
      sum_buffer_(num_hands_, 0),
      num_steps_(num_hands_, 0),
      warm_up_iterations_(warm_up_iterations) {

  const auto legal_actions = round_state->legal_actions();  // the actions you are allowed to take

  // the number of chips you have contributed to the pot this round of betting
  const int my_stack = round_state->stacks[player_id];  // the number of chips you have remaining

  if (ranges::contains(legal_actions, Action::Type::RAISE)) {
    // the smallest and largest numbers of chips for a legal bet/raise
    const auto raise_bounds = round_state->raise_bounds();
    if (raise_bounds[0] <= my_stack) {
      // ToDo: Other bet sizes? Probably another function to add all bet sizes to available actions
      available_actions_.emplace_back(Action::Type::RAISE, raise_bounds[0]);
    }
  }

  for (const auto legal_action : legal_actions) {
    if (legal_action != Action::Type::RAISE) {
      available_actions_.emplace_back(legal_action);
    }
  }

  // Initialize the regret and the strategies and value_
  regrets_ = HandActionsValues(num_hands_, available_actions_.size());
}

float MCCFR::get_child_value(const unsigned hand, const unsigned action) {
  // Can be cached
  throw std::logic_error("Not Implemented");
}

float MCCFR::get_child_value(const unsigned action) {
  // Can be cached
  throw std::logic_error("Not Implemented");
}

std::vector<float> MCCFR::get_root_value() {
  std::vector<float> values(num_hands_, 0);

  for (unsigned hand = 0; hand < num_hands_; hand++) {
    for (unsigned action = 0; action < available_actions_.size(); action++) {
      values[hand] += (sum_buffer_[hand] > 0 ? regrets_(hand, action) / sum_buffer_[hand]
                                             : 1.0f / available_actions_.size()) *
                      get_child_value(hand, action);
    }
  }

  return values;
}

void MCCFR::update_regrets() {
  // ToDo: Not optimal
  values_ = get_root_value();
  // ToDo: Add epsilon-greedy selection?

  // sample a hand
  const unsigned hand = sample_index(ranges_[player_].range, random_generator_);

  // sample an action
  const unsigned action = [&]{
    if(num_steps_[hand] < warm_up_iterations_) {
      return sample_index(regrets_.data, hand * available_actions_.size(),
                                        available_actions_.size(), random_generator_);
    }else {
      return sample_index(available_actions_.size(), random_generator_);
    }
  }();

  const auto& action_value = get_child_value(action, hand);

  // Update regrets
  const float diff = action_value - values_[hand];
  if (diff > 0) {
    regrets_(hand, action) += diff * get_linear_cfr_discount_factor(hand);
    sum_buffer_[hand] += diff * get_linear_cfr_discount_factor(hand);
  }

  ++num_steps_[hand];
}

HandActionsValues MCCFR::get_last_strategy() {
  HandActionsValues strategy(num_hands_, available_actions_.size());
  for (unsigned hand = 0; hand < num_hands_; hand++) {
    for (unsigned action = 0; action < available_actions_.size(); action++) {
      strategy(hand, action) = sum_buffer_[hand] > 0 ? regrets_(hand, action) / sum_buffer_[hand]
                                                     : 1.0f / available_actions_.size();
    }
  }
  return strategy;
}

float MCCFR::get_linear_cfr_discount_factor(const unsigned hand) const {
  // We always have uniform strategy, hence +1.
  const auto num_iterations = static_cast<float>(num_steps_[hand] + 1);

  return num_iterations / (num_iterations + 1);
}

void MCCFR::step() {
  // 1) Set CF values of all terminal and pseudo leaf nodes
  // precompute_all_leaf_values();

  // 3) Compute cumulative regrets and counterfactual values.
  // and generate strategy profile from the regrets
  // (updates `last_strategies`)
  update_regrets();

  // 4) Compute new `reach_probabilities` according to `last_strategies`
  // precompute_reaches_last_strategy(traverser, allow_skips);
}

}  // namespace pokerbot