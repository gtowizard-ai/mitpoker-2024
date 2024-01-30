#!/bin/bash

if [[ "$(uname)" == "Linux" ]]; then
  j=$(nproc)
elif [[ "$(uname)" == "Darwin" ]]; then
  j=$(sysctl -n hw.logicalcpu)
fi

cmake --preset build-pokerbot &&
cmake --build build --target pokerbot -j $j
