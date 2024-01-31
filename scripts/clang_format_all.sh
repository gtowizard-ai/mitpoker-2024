#!/usr/bin/env bash

set -e

ROOT="$(dirname "$0")/../"

clang-format-14 -style=file -i $(find "$ROOT/csrc/src" "$ROOT/csrc/scripts" "$ROOT/simple_bots" -name "*.cc" -o -name '*.cpp' -o -name '*.h')
