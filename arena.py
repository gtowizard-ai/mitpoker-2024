import math

from engine import Game, Player, BIG_BLIND
import json
import statistics
from dataclasses import dataclass

NUM_HANDS = 10000


@dataclass
class MatchResults:
    winrate: float  # in mbb/game
    stddev: float  # in mbb/game


def _run_match(player_1: Player, player_2: Player):
    """Based on `run()` in engine.py"""
    players = [player_1, player_2]
    for player in players:
        player.build()
        player.run()

    game = Game()
    winnings_per_hand = []
    prev_bankroll = player_1.bankroll
    for _ in range(NUM_HANDS):
        game.run_round(players)
        winnings_per_hand.append(player_1.bankroll - prev_bankroll)
        prev_bankroll = player_1.bankroll
        # Switch players position
        players = players[::-1]

    for player in players:
        player.stop()

    assert len(winnings_per_hand) == NUM_HANDS

    bb_per_hand = [x / float(BIG_BLIND) for x in winnings_per_hand]
    stddev_match = math.sqrt(NUM_HANDS * statistics.variance(bb_per_hand))
    results = MatchResults(
        winrate=statistics.mean(bb_per_hand),
        stddev=stddev_match / NUM_HANDS,
    )

    print(f"Results: {results.winrate:.2f} ± {results.stddev:.2f} bb/hand")

    return results


def run_benchmark_vs_check_call_bot():
    main_bot = Player(name="main", path="./csrc/main_bot")
    check_call_bot = Player(name="check_or_call", path="./csrc/check_or_call_bot")
    results = _run_match(main_bot, check_call_bot)

    return {
        "name": "Results vs. Check/Call Bot",
        "unit": "bb/hand",
        "value": round(results.winrate, 4),
        "range": round(results.stddev, 4),
    }


def run_match_vs_bid_everything_bot():
    main_bot = Player(name="main", path="./csrc/main_bot")
    bid_everything_bot = Player(
        name="bid_everything_bot", path="./csrc/bid_everything_bot"
    )
    results = _run_match(main_bot, bid_everything_bot)

    return {
        "name": "Results vs. Bid Everything Bot",
        "unit": "bb/hand",
        "value": round(results.winrate, 4),
        "range": round(results.stddev, 4),
    }


def main():
    results = [
        run_benchmark_vs_check_call_bot(),
        run_match_vs_bid_everything_bot(),
    ]

    with open("arena_results.json", "w") as f:
        json.dump(results, f, indent=4)


if __name__ == "__main__":
    main()
