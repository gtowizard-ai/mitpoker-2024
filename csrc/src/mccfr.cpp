#include "mccfr.h"
#include "ranges_utils.h"

namespace pokerbot {

HandActionsValues::HandActionsValues(const unsigned num_hands, const unsigned num_actions)
    : num_hands_(num_hands) {
  data.reserve(num_hands_ * num_actions);
  std::fill(data.begin(), data.end(), 0);
}

// ToDo: Using rgn can be time consuming. Consider using faster/less accurate methods
// ToDo: Unify the sampling functions
unsigned sample_index(const std::vector<float>& distribution_values, const unsigned start_point,
                      const unsigned length, std::mt19937& gen) {
  std::discrete_distribution<unsigned> distribution(
      distribution_values.begin() + start_point,
      distribution_values.begin() + start_point + length);
  return distribution(gen);
}

unsigned sample_index(const unsigned length, std::mt19937& gen) {
  std::uniform_int_distribution<unsigned> distribution(0, length);
  return distribution(gen);
}

unsigned sample_index(const std::array<float, NUM_HANDS_POSTFLOP_3CARDS>& distribution_values,
                      std::mt19937& gen) {
  std::discrete_distribution<unsigned> distribution(distribution_values.begin(),
                                                    distribution_values.end());
  return distribution(gen);
}

MCCFR::MCCFR(const GameInfo& game_state, const unsigned warm_up_iterations)
    : game_(game_state),
      random_generator_(std::random_device()()),
      warm_up_iterations_(warm_up_iterations),
      values_(NUM_HANDS_POSTFLOP_3CARDS, 0),
      sum_buffer_(NUM_HANDS_POSTFLOP_3CARDS, 0),
      num_steps_(NUM_HANDS_POSTFLOP_3CARDS, 0),
      regrets_(NUM_HANDS_POSTFLOP_3CARDS, max_available_actions_) {}

void MCCFR::build_tree(const RoundStatePtr& round_state) {
  const auto legal_actions = round_state->legal_actions();  // the actions you are allowed to take

  // the number of chips you have contributed to the pot this round of betting
  const int my_stack = round_state->stacks[player_id_];  // the number of chips you have remaining

  for (const auto legal_action : legal_actions) {
    if (legal_action != Action::Type::RAISE) {
      available_actions_.emplace_back(legal_action);
    } else {
      // the smallest and largest numbers of chips for a legal bet/raise
      if (const auto raise_bounds = round_state->raise_bounds(); raise_bounds[0] <= my_stack) {
        // ToDo: Other bet sizes? Probably another function to add all bet sizes to available actions
        available_actions_.emplace_back(Action::Type::RAISE, raise_bounds[0]);
      }
    }
  }
}

float MCCFR::get_child_value(const unsigned hand, const unsigned action) {
  // Can be cached
  throw std::logic_error("Not Implemented");
}

float MCCFR::get_child_value(const unsigned action) {
  // Can be cached
  throw std::logic_error("Not Implemented");
}

void MCCFR::update_root_value() {
  for (unsigned hand = 0; hand < num_hands_; hand++) {
    double root_value = 0;
    for (unsigned action = 0; action < available_actions_.size(); action++) {
      root_value += (sum_buffer_[hand] > 0 ? regrets_(hand, action) / sum_buffer_[hand]
                                           : 1.0f / available_actions_.size()) *
                    get_child_value(hand, action);
    }
    values_[hand] = static_cast<float>(root_value);
  }
}

void MCCFR::update_regrets(const std::vector<Range>& ranges) {
  // ToDo: Add epsilon-greedy selection?
  update_root_value();

  // sample a hand
  const unsigned hand = sample_index(ranges[player_id_].range, random_generator_);

  // sample an action - if there is warm-up state sample uniformely
  const unsigned action = [&] {
    if (num_steps_[hand] < warm_up_iterations_) {
      return sample_index(regrets_.data, hand * available_actions_.size(),
                          available_actions_.size(), random_generator_);
    } else {
      return sample_index(available_actions_.size(), random_generator_);
    }
  }();

  const auto& action_value = get_child_value(action, hand);

  // Update regrets
  if (const float diff = action_value - values_[hand]; diff > 0) {
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

void MCCFR::step(const std::vector<Range>& ranges) {
  // 1) Set CF values of all terminal and pseudo leaf nodes
  // precompute_all_leaf_values();

  // 2) Compute cumulative regrets and counterfactual values.
  // and generate strategy profile from the regrets
  update_regrets(ranges);

  // 3) Question: Should I update the reach/range probs?
}

void MCCFR::initial_regrets() {
  // Iterate over all hands and actions once to prevent biased selection of actions in sampling
  update_root_value();
  for (unsigned hand = 0; hand < num_hands_; hand++) {
    for (unsigned action = 0; action < available_actions_.size(); action++) {
      const auto& action_value = get_child_value(action, hand);
      // Update regrets
      if (const float diff = action_value - values_[hand]; diff > 0) {
        regrets_(hand, action) += diff * get_linear_cfr_discount_factor(hand);
        sum_buffer_[hand] += diff * get_linear_cfr_discount_factor(hand);
      }
    }
  }
}

void MCCFR::solve(const std::vector<Range>& ranges, const RoundStatePtr& round_state,
                  const unsigned player_id /*, time_budget*/) {
  // Initialize variable for this solve
  num_hands_ = ranges[player_id].range.size();
  player_id_ = player_id;

  std::fill_n(values_.begin(), num_hands_, 0);
  std::fill_n(sum_buffer_.begin(), num_hands_, 0);
  std::fill_n(num_steps_.begin(), num_hands_, 0);

  build_tree(round_state);

  initial_regrets();

  // FIXME: compute the number of iteration based on time budget
  int max_iterations = 10000;
  while (max_iterations-- > 0) {
    step(ranges);
  }
}

}  // namespace pokerbot