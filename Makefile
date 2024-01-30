# Check the operating system
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
  export CC ?= /usr/bin/gcc
  export CXX ?= /usr/bin/g++
  j := $(shell nproc)
else ifeq ($(UNAME), Darwin)
  export CC := /opt/homebrew/opt/llvm/bin/clang
  export CXX := /opt/homebrew/opt/llvm/bin/clang++
  j := $(shell sysctl -n hw.logicalcpu)
endif

# Check if `ninja` is installed
ifeq (, $(shell which ninja))
  CONFIGURE_PRESET := unix-makefiles
else
  CONFIGURE_PRESET := default
endif

# List all `CMakeLists.txt` files
PREREQUISITES := $(shell find . -name CMakeLists.txt -print -o -name CMakePresets.json -print)

.PHONY: all configure configure_bot bot compile test clean

all: compile

csrc/build/.timestamp: $(PREREQUISITES)
	cmake -S csrc --preset $(CONFIGURE_PRESET)

csrc/build/.timestamp-bot: $(PREREQUISITES)
	cmake -S csrc --preset build-pokerbot

configure:
	cmake -S csrc --preset $(CONFIGURE_PRESET) --fresh

configure_bot:
	cmake -S csrc --preset build-pokerbot --fresh

bot: csrc/build/.timestamp-bot
	cmake --build csrc/build --target pokerbot -j $(j)

compile: csrc/build/.timestamp
	cmake --build csrc/build -j $(j)

test: compile
	cd csrc && ctest --preset default

clean:
	rm -rf csrc/build simple_bots/build
