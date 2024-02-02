#include "main_bot.h"
#include "ranges_utils.h"

namespace pokerbot {

MainBot::MainBot()
    : game_(), auctioneer_(), cfr_(game_), time_manager_(), gen_(std::random_device{}()) {}

Action MainBot::sample_action(const GameInfo& game_info, const RoundState& state, const Hand& hand,
                              const int hero_id, const HandActionsValues& strategy,
                              const std::vector<Action>& legal_actions,
                              const float min_prob_sampling) const {
  if (legal_actions.size() != strategy.num_actions) {
    throw std::runtime_error("Actions mistmatch");
  }

  // Get strategy for hand
  std::vector<float> probs;
  probs.reserve(strategy.num_actions);
  for (unsigned a = 0; a < strategy.num_actions; ++a) {
    probs.push_back(strategy(a, hand.index()));
  }
  // Don't sample if prob is too low due to non-convergence
  for (auto& val : probs) {
    if (val < min_prob_sampling) {
      val = 0;
    }
  }

  std::discrete_distribution<> dis(probs.begin(), probs.end());
  const int sampled_idx = dis(gen_);
  const auto sampled_action = legal_actions[sampled_idx];
  std::string strategy_str;
  for (auto val : probs) {
    strategy_str += std::to_string(val) + ",";
  }
  fmt::print(
      "#{} - {} - {} - (Hero={}, Pot={}, Bets={}/{}) "
      "Sampled Action {} on {} based on strategy = {} \n",
      game_info.hand_num, state.round().to_string(), hand.to_string(), hero_id, state.pot(),
      state.bets[0], state.bets[1], sampled_action.to_string(), Card::to_string(state.board_cards),
      strategy_str);

  return sampled_action;
}

void MainBot::update_range(const int player, const HandActionsValues& strategy,
                           const std::vector<Action>& legal_actions, const Action& action,
                           const std::optional<Hand>& player_hand, const float min_prob_keep) {
  if (legal_actions.size() != strategy.num_actions) {
    throw std::runtime_error("Actions mistmatch");
  }
  auto it = std::find(legal_actions.begin(), legal_actions.end(), action);
  if (it == legal_actions.end()) {
    throw std::runtime_error(fmt::format("Actions mistmatch Two: {}", action.to_string()));
  }
  auto action_idx = std::distance(legal_actions.begin(), it);

  if (player_hand.has_value()) {
    const auto hero_hand_idx = player_hand->index();
    for (hand_t i = 0; i < ranges_[player].num_hands(); ++i) {
      const auto action_prob = strategy(action_idx, i);
      ranges_[player].range[i] *= action_prob;
      if (ranges_[player].range[i] < min_prob_keep && i != hero_hand_idx) {
        ranges_[player].range[i] = 0;
      }
    }
  } else {
    for (hand_t i = 0; i < ranges_[player].num_hands(); ++i) {
      const auto action_prob = strategy(action_idx, i);
      auto strat = (1.0f - WEIGHT_UNIFORM_RANDOM_RANGE_OPPONENT) * action_prob +
                   WEIGHT_UNIFORM_RANDOM_RANGE_OPPONENT * (1 - action_prob);
      ranges_[player].range[i] *= strat;
    }
  }
}

void MainBot::handle_new_hand(const GameInfo& /*game_info*/, const RoundStatePtr& /*state*/,
                              int /*active*/) {
  std::fill(ranges_.begin(), ranges_.end(), Range());
}

void MainBot::handle_hand_over(const GameInfo& /*game_info*/,
                               const TerminalStatePtr& /*terminal_state*/, int /*active*/) {}

std::optional<Action> get_last_opponent_action(const RoundState& curr_state,
                                               const RoundState& prev_state, const int opp_index) {
  // Not sure if there is a better way to get this based on `RoundState`..
  if (curr_state.bets[opp_index] > prev_state.bets[opp_index] &&
      curr_state.bets[opp_index] > curr_state.bets[1 - opp_index]) {
    return Action{Action::Type::RAISE, curr_state.bets[opp_index]};
  }
  if (curr_state.bets[opp_index] > prev_state.bets[opp_index] &&
      curr_state.bets[opp_index] == curr_state.bets[1 - opp_index]) {
    return Action{Action::Type::CALL};
  }
  if (curr_state.bets[opp_index] == prev_state.bets[opp_index]) {
    return Action{Action::Type::CHECK};
  }
  return std::nullopt;
}

Action MainBot::get_action(const GameInfo& game_info, const RoundStatePtr& state,
                           const int active) {
  if (state->previous_state != nullptr) {
    // Simulate the thinking process of our opponent if he made an action
    const auto last_decision_node =
        std::dynamic_pointer_cast<const RoundState>(state->previous_state);
    if (last_decision_node != nullptr) {
      const auto opp_index = get_active(last_decision_node->button);
      const auto is_opponent_node = opp_index != active;
      auto last_action = get_last_opponent_action(*state, *last_decision_node, opp_index);
      if (is_opponent_node && last_action.has_value() &&
          last_decision_node->legal_actions().size() > 1) {
        get_action_any_player(game_info, last_decision_node, opp_index, last_action);
      }
    }
  }
  return get_action_any_player(game_info, state, active, std::nullopt);
}

Action MainBot::solve_preflop_root_node(const GameInfo& game_info, const RoundStatePtr& state,
                                        const int player, const bool is_hero_node,
                                        std::optional<Action> sampled_action,
                                        const std::optional<Hand>& player_hand) {
  bool action_in_cache = is_hero_node;
  auto& sb_legal_actions =
      is_hero_node ? hero_sb_cached_legal_actions_ : opp_sb_cached_legal_actions_;
  auto& sb_strategy = is_hero_node ? hero_sb_cached_strategy_ : opp_sb_cached_strategy_;
  if (sampled_action.has_value() && sb_legal_actions.has_value()) {
    for (const auto& action : *sb_legal_actions) {
      if (action.type == sampled_action->type && action.amount == sampled_action->amount) {
        action_in_cache = true;
      }
    }
  }

  if (!action_in_cache || !sb_legal_actions.has_value() || !sb_strategy.has_value()) {
    // Solve with a larger time limit (computed once)
    float time_allowed = is_hero_node ? 100 : 15;
    fmt::print("is_hero={} - Solving root preflop node for {}ms \n", is_hero_node, time_allowed);
    cfr_.solve(ranges_, state, player, time_allowed, sampled_action, 400);
    sb_strategy = cfr_.strategy();
    sb_legal_actions = cfr_.legal_actions();
  }
  if (is_hero_node) {
    sampled_action =
        sample_action(game_info, *state, *player_hand, player, *sb_strategy, *sb_legal_actions);
  }
  update_range(player, *sb_strategy, *sb_legal_actions, *sampled_action, player_hand);
  return *sampled_action;
}

Action MainBot::solve_with_cfr(const GameInfo& game_info, const RoundStatePtr& state,
                                        const int player, const bool is_hero_node,
                                        std::optional<Action> sampled_action,
                                        const std::optional<Hand>& player_hand) {
  // set time budget
  time_manager_.update_action(game_info, state);
  const auto time_budget_ms = time_manager_.get_time_budget_ms(game_info, state);
  ranges_[player].update_on_board_cards(game_, state->board_cards);
  ranges_[1 - player].update_on_board_cards(game_, state->board_cards);

  fmt::print("is_hero={} - {:.2f} ms allocated for solving with CFR \n", is_hero_node,
             time_budget_ms);
  cfr_.solve(ranges_, state, player, time_budget_ms, sampled_action, 500);
  if (is_hero_node) {
    sampled_action = sample_action(game_info, *state, *player_hand, player, cfr_.strategy(),
                                   cfr_.legal_actions());
  }
  update_range(player, cfr_.strategy(), cfr_.legal_actions(), *sampled_action, player_hand);
  return *sampled_action;
}

Action MainBot::get_action_any_player(const GameInfo& game_info, const RoundStatePtr& state,
                                      const int player, std::optional<Action> sampled_action) {
  const auto legal_actions = state->legal_actions();

  std::optional<Hand> player_hand = std::nullopt;
  const bool is_hero_node = !sampled_action.has_value();
  if (is_hero_node) {
    player_hand = Hand(state->hands[player]);
  }

  if (is_hero_node && ranges::contains(legal_actions, Action::Type::BID)) {
    /// Auction
    const auto bid = auctioneer_.get_bid(state->board_cards, *player_hand, state->pot());
    fmt::print("Bidding {} in {}\n", bid, state->pot());
    return {Action::Type::BID, bid};
  }

  if (is_hero_node && state->bids[player].has_value() && state->bids[1 - player].has_value()) {
    /// Right after auction
    /// NB - Careful this is called on *every action* after auction happened
    auctioneer_.receive_bid(ranges_[player], ranges_[1 - player], *state->bids[player],
                            *state->bids[1 - player], game_, state->board_cards, state->pot());
  }

  // NB: Put this constraint after checking if only legal action is bid!
  if (legal_actions.size() == 1) {
    // Engine keeps asking to take an action even when both players are all-in..
    // Don't waste any time and return
    return Action{state->legal_actions().front()};
  }

  if (state->round() == round::FLOP && state->bets[1 - player] == 0 && player == BB_POS) {
    fmt::print("OOP Flop - Checking 100% of the time \n");
    return Action{Action::Type::CHECK};
  }
  if (is_hero_node && state->round() == round::TURN && state->bets[1 - player] == 0 &&
      player == BB_POS && player_hand->num_cards() == 2) {
    fmt::print("OOP Turn with 2 cards - Checking 100% of the time \n");
    return Action{Action::Type::CHECK};
  }

  if (state->round() == round::PREFLOP && player == SB_POS &&
      state->bets[1 - player] == BIG_BLIND) {
    return solve_preflop_root_node(game_info, state, player, is_hero_node, sampled_action, player_hand);
  } else {
    return solve_with_cfr(game_info, state, player, is_hero_node, sampled_action, player_hand);
  }
}

}  // namespace pokerbot
