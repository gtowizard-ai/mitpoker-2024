cmake_minimum_required(VERSION 3.21)

project(simple-bot)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Enable colored diagnostics for Ninja
if(CMAKE_GENERATOR STREQUAL "Ninja")
    set(CMAKE_COLOR_DIAGNOSTICS ON)
    add_link_options($<IF:$<STREQUAL:$<CXX_COMPILER_ID>,GNU>,-fdiagnostics-color=always,-fcolor-diagnostics>)
endif()

# Add compiler warning flags
add_compile_options(-Wfatal-errors -Wall -Wextra -Wnon-virtual-dtor -pedantic-errors -Wno-missing-braces)

# Add source files
add_subdirectory(../csrc csrc)

# Executables
add_executable(check_or_call_bot check_or_call_bot/check_or_call_bot.cpp)
target_link_libraries(check_or_call_bot dependencies gtowizardai)
target_include_directories(check_or_call_bot PRIVATE ../csrc)

add_executable(bid_everything_bot bid_everything_bot/bid_everything_bot.cpp)
target_link_libraries(bid_everything_bot dependencies gtowizardai)
target_include_directories(bid_everything_bot PRIVATE ../csrc)

add_executable(uniform_random_bot uniform_random_bot/uniform_random_bot.cpp)
target_link_libraries(uniform_random_bot dependencies gtowizardai)
target_include_directories(uniform_random_bot PRIVATE ../csrc)

add_executable(preflop_all_in_bot preflop_all_in_bot/preflop_all_in_bot.cpp)
target_link_libraries(preflop_all_in_bot dependencies gtowizardai)
target_include_directories(preflop_all_in_bot PRIVATE ../csrc)
