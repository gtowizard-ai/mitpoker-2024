#!/usr/bin/env bash

set -e

ROOT="$(dirname "$0")/../"

clang-format-14 -style=file -i $(find "$ROOT/csrc" -name "*.cc" -o -name '*.h')
clang-format-14 -style=file -i $(find "$ROOT/lib/ruse_tools" -name "*.cc" -o -name '*.h')
