## Poker hand evaluator (`poker_hand.h`)

`scripts/generate_rank_bases.cpp` and `scripts/generate_evaluator_tables.cpp` are used to generate a poker hand evaluator that calculates information about hand strength, flush draw or not, and number of straight draw outs.
The logic is based on the [OMPEval] library, but supports 5 to 8 card hands (specialized for Auction Hold'em!).
It is extremely efficient, and it can evaluate 400 million random hands per second with a single thread (on M3 Max MacBook Pro).

[OMPEval]: https://github.com/zekyll/OMPEval

## Counterfactual value calculator for terminal nodes (`equity.h`, `equity.cpp`)

The `compute_cfvs_showdown` function calculates counterfactual values for "terminal nodes".
Before calculating the counterfactual values, we need to evaluate all possible hands and sort them by strength, which takes O(n log n) time (n = 22,100 = (52 choose 3)).
Once sorted, `compute_cfvs_showdown` runs in O(n) time, performing 6,000 calculations per second for cases where both players have three hole cards (on the same MacBook Pro).
When either of the two players has folded, we use `compute_cfvs_fixed_payoff` instead to calculate counterfactual values more efficiently.

Here, "terminal nodes" may not refer to the actual end of the game.
On the flop and turn, "terminal values" are calculated as if the game ends on the current street, i.e., there are
no more betting rounds.
This abstraction makes draw hands undervalued, so we added a special treatment to "boost" the values of flush draws and straight draws.
However, this treatment is only a heuristic and does not necessarily give well-defined values, and also it turns out that it does not seem to improve performance significantly.
