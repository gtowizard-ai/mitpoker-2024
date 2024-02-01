#include "main_bot.h"
#include "ranges_utils.h"

namespace pokerbot {

MainBot::MainBot()
    : game_(), auctioneer_(), cfr_(game_), time_manager_(), gen_(std::random_device{}()) {}

Action MainBot::sample_action(const GameInfo& game_info, const RoundState& state, const Hand& hand,
                              const int hero_id, const HandActionsValues& strategy,
                              const std::vector<Action>& legal_actions,
                              const float min_prob_sampling) {
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
      "#{} - {} - {} - (Hero={}, Pot={}, Bets={}/{}) Sampled Action {} on {} based on strategy = "
      "{} \n",
      game_info.hand_num, state.round().to_string(), hand.to_string(), hero_id, state.pot(),
      state.bets[0], state.bets[1], sampled_action.to_string(), Card::to_string(state.board_cards),
      strategy_str);

  return sampled_action;
}

void MainBot::update_range(const int player, const HandActionsValues& strategy,
                           const std::vector<Action>& legal_actions, const Action& action) {
  if (legal_actions.size() != strategy.num_actions) {
    throw std::runtime_error("Actions mistmatch");
  }
  unsigned action_idx;
  for (action_idx = 0; action_idx < legal_actions.size(); ++action_idx) {
    if (legal_actions[action_idx].amount == action.amount &&
        legal_actions[action_idx].type == action.type) {
      break;
    }
  }
  if (action_idx == legal_actions.size()) {
    std::string actions;
    for (const auto& action : legal_actions) {
      actions += action.to_string() + ",";
    }
    throw std::runtime_error(
        fmt::format("Actions mistmatch Two: {} ({})", action.to_string(), actions));
  }

  // FIXME ADD UNIFORM RANDOM?
  for (hand_t i = 0; i < ranges_[player].num_hands(); ++i) {
    const auto action_prob = strategy(action_idx, i);
    ranges_[player].range[i] *= action_prob;
  }
}

void MainBot::handle_new_hand(const GameInfo& game_info, const RoundStatePtr& /*state*/,
                              int /*active*/) {
  std::fill(ranges_.begin(), ranges_.end(), Range());
  fmt::print("Time Remaining:: {}\n", game_info.game_clock);
}

void MainBot::handle_hand_over(const GameInfo& /*game_info*/,
                               const TerminalStatePtr& /*terminal_state*/, int /*active*/) {
  // int my_delta = terminal_state->deltas[active];  // your bankroll change from this round
  // auto previous_state = std::static_pointer_cast<const RoundState>(terminal_state->previous_state);  // RoundState before payoffs
  // auto my_cards = previous_state->hands[active];  // your cards
  // auto opp_cards = previous_state->hands[1-active];  // opponent's cards or "" if not revealed
}

Action MainBot::get_action(const GameInfo& game_info, const RoundStatePtr& state,
                           const int active) {
  if (state->previous_state != nullptr) {
    const auto last_decision_node =
        std::dynamic_pointer_cast<const RoundState>(state->previous_state);
    if (last_decision_node != nullptr) {
      const auto opp_index = get_active(last_decision_node->button);
      const auto is_opponent_node = opp_index != active;
      fmt::print("Last decision node: Opponent = {} - {} \n", is_opponent_node,
                 last_decision_node->to_string());
      // I don't know a better way to get this..
      const std::optional<Action> last_action = [&]() -> std::optional<Action> {
        if (state->bets[opp_index] > last_decision_node->bets[opp_index] &&
            state->bets[opp_index] > state->bets[1 - opp_index]) {
          return Action{Action::Type::RAISE, state->bets[opp_index]};
        }
        if (state->bets[opp_index] > last_decision_node->bets[opp_index] &&
            state->bets[opp_index] == state->bets[1 - opp_index]) {
          return Action{Action::Type::CALL};
        }
        if (state->bets[opp_index] == last_decision_node->bets[opp_index]) {
          return Action{Action::Type::CHECK};
        }
        return std::nullopt;
        ;
      }();
      if (is_opponent_node && last_action.has_value()) {
        get_action_any_player(game_info, last_decision_node, opp_index, last_action);
      }
    }
  }
  return get_action_any_player(game_info, state, active, std::nullopt);
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
    const auto bid = auctioneer_.get_bid(ranges_[player], ranges_[1 - player], game_,
                                         state->board_cards, *player_hand, state->pot());
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

  if (is_hero_node && state->round() == round::PREFLOP && player == SB_POS &&
      state->bets[1 - player] == BIG_BLIND) {
    if (!preflop_sb_cached_strategy_[player].has_value() ||
        !preflop_sb_cached_legal_actions_[player].has_value()) {
      // Solve with a larger time limit (computed once)
      fmt::print("Solving root preflop node for 100ms \n");
      cfr_.solve(ranges_, state, player, 100, sampled_action);
      preflop_sb_cached_strategy_[player] = cfr_.strategy();
      preflop_sb_cached_legal_actions_[player] = cfr_.legal_actions();
    }
    if (is_hero_node) {
      sampled_action = sample_action(game_info, *state, *player_hand, player,
                                     *preflop_sb_cached_strategy_[player],
                                     *preflop_sb_cached_legal_actions_[player]);
    }
    update_range(player, *preflop_sb_cached_strategy_[player],
                 *preflop_sb_cached_legal_actions_[player], *sampled_action);

  } else {
    // set time budget
    time_manager_.update_action(game_info, state);
    const auto time_budget_ms = time_manager_.get_time_budget_ms(game_info, state);
    // FIXME.. Could be avoided
    ranges_[player].update_on_board_cards(game_, state->board_cards);
    ranges_[1 - player].update_on_board_cards(game_, state->board_cards);

    // Solve..
    // FIXME - On flop/turn, CFVs are calculated as if showdown will be held with the current board
    // (i.e., no more chance cards are dealt)
    fmt::print("is_hero={} - {:.2f} ms allocated for solving with CFR \n", is_hero_node,
               time_budget_ms);
    cfr_.solve(ranges_, state, player, time_budget_ms, sampled_action);
    if (is_hero_node) {
      sampled_action = sample_action(game_info, *state, *player_hand, player, cfr_.strategy(),
                                     cfr_.legal_actions());
    }
    update_range(player, cfr_.strategy(), cfr_.legal_actions(), *sampled_action);
  }

  return *sampled_action;
}

}  // namespace pokerbot
