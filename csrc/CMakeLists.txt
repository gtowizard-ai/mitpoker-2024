cmake_minimum_required(VERSION 3.2)

project(gto-wizard-ai LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Only Linux+GCC, Linux+Clang, and macOS+Clang are supported
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(WARNING "Only GCC and Clang are supported on Linux (Detected compiler: ${CMAKE_CXX_COMPILER_ID})")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(WARNING "Only Homebrew Clang is supported on macOS (Detected compiler: ${CMAKE_CXX_COMPILER_ID})")
    endif()
else()
    message(WARNING "Unsupported system: ${CMAKE_SYSTEM_NAME} (only Linux and macOS are supported)")
endif()

# Set a default build type if none was specified
# https://blog.kitware.com/cmake-and-the-default-build-type/
set(default_build_type "Release")  # "Debug" / "Release" / "MinSizeRel" / "RelWithDebInfo"
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
endif()
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-march=native)
endif()


file(GLOB_RECURSE BOT_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(pokerbot ${BOT_SRC})
target_include_directories(pokerbot PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(pokerbot skeleton)

# Generate position-independent code (-fPIC / -fPIE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable colored diagnostics for Ninja
if(CMAKE_GENERATOR STREQUAL "Ninja")
    set(CMAKE_COLOR_DIAGNOSTICS ON)
    add_link_options($<IF:$<STREQUAL:$<CXX_COMPILER_ID>,GNU>,-fdiagnostics-color=always,-fcolor-diagnostics>)
endif()


# External libraries
enable_testing()
add_subdirectory(../third_party third_party EXCLUDE_FROM_ALL SYSTEM)
add_library(dependencies INTERFACE)
target_link_libraries(dependencies INTERFACE third_party)

# Add compiler warning flags
add_compile_options(-Wfatal-errors -Wall -Wextra -Wnon-virtual-dtor -pedantic-errors)

# Precompiled headers (in the order at https://en.cppreference.com/w/cpp/header for STL headers)
set(precompiled_headers
    <chrono> <functional> <optional> <tuple> <utility> <memory>
    <cinttypes> <cstdint> <limits> <cassert> <stdexcept> <string> <string_view>
    <array> <map> <set> <span> <unordered_map> <unordered_set> <vector> <ranges>
    <algorithm> <cmath> <numeric> <random> <cstdio> <fstream> <iomanip> <sstream>
    <mutex> <thread> <fmt/core.h> <fmt/ranges.h>)

# Tests
add_executable(googletest_test googletest_test.cpp)
target_link_libraries(googletest_test dependencies gtest_main)
target_precompile_headers(googletest_test PRIVATE ${precompiled_headers})
add_test(NAME googletest_test COMMAND googletest_test)

# Subdirectories
add_subdirectory(skeleton)
