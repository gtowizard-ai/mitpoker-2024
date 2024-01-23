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
