from engine import Game, Player, BIG_BLIND
import json


NUM_HANDS = 5000

def _run_game(player_1: Player, player_2: Player):
    """ Based on `run()` in engine.py"""
    players = [player_1, player_2]
    for player in players:
        player.build()
        player.run()

    game = Game()
    for _ in range(NUM_HANDS):
        game.run_round(players)
        players = players[::-1]
    for player in players:
        player.stop()

    print(f"Final Bankrolls are {players[0].bankroll}/{players[1].bankroll}")
    return players

def run_benchmark_vs_check_call_bot():
    main_bot = Player(name="main", path="./csrc/main_bot")
    check_call_bot = Player(name="check_or_call", path="./csrc/check_or_call_bot")
    players = _run_game(main_bot, check_call_bot)

    won_in_bb = players[0].bankroll / BIG_BLIND
    return {
        "name": "Results vs. Check/Call Bot",
        "unit": "Big blinds",
        "value": won_in_bb * (100.0 / NUM_HANDS)
    }

def run_match_vs_bid_everything_bot():
    main_bot = Player(name="main", path="./csrc/main_bot")
    bid_everything_bot = Player(name="bid_everything_bot", path="./csrc/bid_everything_bot")
    players = _run_game(main_bot, bid_everything_bot)

    won_in_bb = players[0].bankroll / BIG_BLIND
    return {
        "name": "Results vs. Bid Everything Bot",
        "unit": "BB/100",
        "value": won_in_bb * (100.0 / NUM_HANDS)
    }


def main():
    results = [
        run_benchmark_vs_check_call_bot(),
        run_match_vs_bid_everything_bot(),
    ]

    with open("arena_results.json", "w") as f:
        json.dump(results, f, indent=4)




if __name__ == '__main__':
    main()