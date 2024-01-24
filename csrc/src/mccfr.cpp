#include "mccfr.h"
#include "ranges_utils.h"

namespace pokerbot {

int sample_index(const std::vector<float>& distribution_values, std::mt19937 gen) {
  // Create a discrete distribution based on the values in A
  std::discrete_distribution<int> distribution(distribution_values.begin(),
                                               distribution_values.end());

  // Sample a value
  return distribution(gen);
}

MCCFR::MCCFR(const GameInfo& game_state, const RoundStatePtr& round_state, const int player_id,
             const std::vector<Range>& ranges)
    : game_(game_state),
      round_state_(round_state_),
      player_(player_id),
      ranges_(ranges),
      value_(0),
      random_generator_(std::random_device()()),
      sum_buffer_(0),
      num_steps_(0) {

  const auto legal_actions = round_state->legal_actions();  // the actions you are allowed to take

  // the number of chips you have contributed to the pot this round of betting
  const int my_pip = round_state->pips[player_id];
  const int my_stack = round_state->stacks[player_id];  // the number of chips you have remaining

  int min_cost = 0;
  int max_cost = 0;
  if (ranges::contains(legal_actions, Action::Type::RAISE)) {
    // the smallest and largest numbers of chips for a legal bet/raise
    const auto raise_bounds = round_state->raise_bounds();
    if(raise_bounds[0] <= my_stack) {
      // Question: Other values?
      available_actions_.emplace_back(Action::Type::RAISE, raise_bounds[0]);
    }
  }

  for (const auto legal_action : legal_actions) {
    if (legal_action == Action::Type::BID) {
      // Question: How to handle BID?
      available_actions_.emplace_back(Action::Type::BID, 100);  // random bid
    } else if(legal_action != Action::Type::RAISE) {
      available_actions_.emplace_back(legal_action);
    }
  }

  // Initialize the regret and the strategies and value_
  regrets_.resize(available_actions_.size());
  sum_strategies_.resize(available_actions_.size());
  last_strategies_.resize(available_actions_.size());
  average_strategies_.resize(available_actions_.size());

  std::fill(regrets_.begin(), regrets_.end(), 0);
  std::fill(sum_strategies_.begin(), sum_strategies_.end(), 0);
  std::fill(last_strategies_.begin(), last_strategies_.end(), 1.0f/last_strategies_.size());
  std::fill(average_strategies_.begin(), average_strategies_.end(), 0);
}

float MCCFR::get_child_value(const int action, const int hand) {
  // Can be cached
  throw std::logic_error("Not Implemented");
}

float MCCFR::get_child_value(const int action) {
  // Can be cached
  throw std::logic_error("Not Implemented");
}

void MCCFR::update_regrets() {
  value_ = 0;
  for(int index = 0; index < available_actions_.size(); index++) {
    value_ += last_strategies_[index] * get_child_value(index);
  }
  // Fixme: Add the pos_discount and neg_discount?
  // Fixme: Add epsilon-greedy selection?

  // sample a hand
  const int hand = sample_index(ranges_[player_], random_generator_);

  // sample an action
  const int action = sample_index(average_strategies_, random_generator_);

  // Update regrets for traverser only
  // Update last strategies with Regret Matching
  const auto& action_value = get_child_value(action, hand);

  // Question: Should it be = or += ?
  // Question: I do sampling. Should I update regret_[hand, action] or regret_[action] ?
  regrets_[action] = action_value - value_;
  if (regrets_[action] > 0) {
    sum_buffer_ -= last_strategies_[action];
    last_strategies_[action] = regrets_[action];
    sum_buffer_ += last_strategies_[action];
  } else {
    last_strategies_[action] = 0;
  }

  // Transform to probability distribution
  for (float& last_strategie : last_strategies_) {
    last_strategie = sum_buffer_ > 0 ? static_cast<float>(last_strategie / sum_buffer_)
                                     : 1.0f / last_strategies_.size();
  }
}

void MCCFR::update_sum_strategies(const float strat_discount) {
  for (unsigned action = 0; action < sum_strategies_.size(); ++action) {
    sum_strategies_[action] *= strat_discount;
    sum_strategies_[action] += last_strategies_[action];
  }
}

void MCCFR::compute_avg_strategy() {
  sum_buffer_ = 0;

  for (const float sum_strategy : sum_strategies_) {
    sum_buffer_ += sum_strategy;
  }

  // Transform to probability distribution
  for (unsigned action = 0; action < average_strategies_.size(); ++action) {
    average_strategies_[action] = static_cast<float>(sum_strategies_[action] / sum_buffer_);
  }
}

void MCCFR::step() {
  // 1) Set CF values of all terminal and pseudo leaf nodes
  // precompute_all_leaf_values();

  // 2) Get weights for regrets and average strategies
  // according to our discounting scheme
  // const auto [pos_discount, neg_discount, strat_discount] = get_discount_factors();

  // 3) Compute cumulative regrets and counterfactual values.
  // and generate strategy profile from the regrets
  // (updates `last_strategies`)
  update_regrets();

  // 4) Compute new `reach_probabilities` according to `last_strategies`
  // precompute_reaches_last_strategy(traverser, allow_skips);

  // Update num_steps before strategy to know on which
  // iteration average strategy was updated
  ++num_steps_;

  // 5) Update sum strategies
  compute_avg_strategy();
}

}  // namespace pokerbot