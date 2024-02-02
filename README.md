# MIT Pokerbots 2024 Engine
MIT Pokerbots engine and skeleton bots in Python, Java, and C++.

The command to run the engine is `python3 engine.py`. The engine is configured via `config.py`.

## Dependencies
 - python>=3.12
 - cython (pip install cython)
 - eval7 (pip install eval7)
 - C++17 for cpp_skeleton
 - boost for cpp_skeleton (`sudo apt install libboost-all-dev`)
 - fmt for cpp_skeleton

### Pre-Commit Hooks

We format our code with `clang-format` for the C++ code. To avoid having to commit code multiple times
to pass linting tests, we use pre-commit hooks. To set these up, you should have `pre-commit` installed (this should already be in the python development dependencies), and then you can run

```bash
pre-commit install
```


### The Auction

Before getting into our approach for the auction we should define the difference between expected value and equity:

- Equity: The percentage of the time our hand (or range of hands) will win the pot against our opponent's range. 

- Expected Value (EV): The amount of money a hand (or range) is expected to win. Consider a hand with 100% equity. If we make a pot sized bet with that hand and get called 50% of the time - then the expected value of our hand is 1.5 times the pot (1 from the pot, .5 from when we get called and win). 

We say that a range of hands "overrealizes" its equity if its EV is greater than its equity multiplied by the size of the pot. We say that a hand "underrealizes" its equity if its EV is less than its equity multiplied by the size of the pot.

The optimal approach given the time constraint of 1000 hands in 30 seconds would have almost certainly been to make a blueprint of the game based on an abstraction of the game that is much smaller than the game itself and solving it via MCCFR. 

However, we wanted our approach to scale well without the time constraint - and an abstraction based method does not. This presents a particular problem for the auction because it is early enough in the game tree that the expected value of bidding can't be computed quickly enough (even if we make assumptions about the behavior of both players' pre-flop strategy and only solve the post-flop subgame).

Equity, on the other hand, is an easy proxy for expected value that can be easily computed without solving an entire subgame. We computed "3v2 - 2v3" - i.e. the equity that each hand would have given if it gets the extra card subtracted by the equity that hand would have if the opponent gets the extra card. We did this for every hand on every distinct board against a uniform random range of hands - resulting in a matrix of 1755 x 1326 precomputed equity difference values. 

Just considering equity is not enough. We also need to consider that the player with the third card will often overrealize (this is because they can leverage having better hands on average to force their opponent to fold out his equity - forcing the opponent to underrealize. The idea that a player that has a large density of superior hands can force their opponent to underrealize is a well known principle in poker that is confirmed by solvers of traditional NLHE). This incentivises us to bid higher than our equity alone would dictate.

We elected to represent this overrealization by multiplying our preferred bid by a constant multiple in all spots (this could be made to be more granular - for example the in position player is likely going to overrealize more than the out of position player). In order to represent overrealization - we ran monte carlo simulations to compare the outcomes of different bid multipliers. Bots that won the card for 1.8 times the equity based bid outperformed all bots that won the auction for values between 0.5-2.5. 

So in short our bidding approach was to choose a bid based on the equity of our hand against a random range - then scale that bid to account for the fact that the player with 3 cards will likely overrealize their equity. 

### Arena Benchmarks

Find our [benchmarks here](https://github.com/ruse-ai/mitpokerbots-2024/tree/gh-pages)
