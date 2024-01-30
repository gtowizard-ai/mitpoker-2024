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

# List all `CMakeLists.txt` files and `third_party/*` directories, excluding `third_party/*/*`
PREREQUISITES := $(shell find . -path './third_party/*/*' -prune -o -name CMakeLists.txt -print -o \
                   -name CMakePresets.json -print -o -path './third_party/*' -type d -print)

.PHONY: all configure configure_bot bot compile test clean

all: compile

build/.timestamp: $(PREREQUISITES)
	cmake -S csrc --preset $(CONFIGURE_PRESET)

build/.timestamp-bot: $(PREREQUISITES)
	cmake -S csrc --preset build-pokerbot

configure:
	cmake -S csrc --preset $(CONFIGURE_PRESET) --fresh

configure_bot:
	cmake -S csrc --preset build-pokerbot --fresh

bot: build/.timestamp-bot
	cmake --build build --target pokerbot -j $(j)

compile: build/.timestamp
	cmake --build build -j $(j)

test: compile
	cd csrc && ctest --preset default

clean:
	rm -rf build
