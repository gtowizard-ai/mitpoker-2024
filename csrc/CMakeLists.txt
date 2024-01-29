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

# Generate position-independent code (-fPIC / -fPIE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable colored diagnostics for Ninja
if(CMAKE_GENERATOR STREQUAL "Ninja")
    set(CMAKE_COLOR_DIAGNOSTICS ON)
    add_link_options($<IF:$<STREQUAL:$<CXX_COMPILER_ID>,GNU>,-fdiagnostics-color=always,-fcolor-diagnostics>)
endif()

# Add compiler warning flags
add_compile_options(-Wfatal-errors -Wall -Wextra -Wnon-virtual-dtor -pedantic-errors)
add_compile_options($<$<STREQUAL:$<CXX_COMPILER_ID>,Clang>:-Wno-gnu-zero-variadic-macro-arguments>)

# External libraries
enable_testing()
add_subdirectory(../third_party third_party EXCLUDE_FROM_ALL SYSTEM)
add_library(dependencies INTERFACE)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREAD OFF)
find_package(Boost REQUIRED system)
include_directories(${Boost_INCLUDE_DIR})

target_link_libraries(dependencies INTERFACE third_party dependencies ${Boost_SYSTEM_LIBRARY})

# Main library
add_library(gtowizardai OBJECT)
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(FILTER sources EXCLUDE REGEX .*_test\.cpp$)
target_sources(gtowizardai PRIVATE ${sources})
target_link_libraries(gtowizardai PRIVATE dependencies)

# Executables
add_executable(check_or_call_bot check_or_call_bot/check_or_call_bot.cpp)
target_link_libraries(check_or_call_bot dependencies gtowizardai)

add_executable(bid_everything_bot bid_everything_bot/bid_everything_bot.cpp)
target_link_libraries(bid_everything_bot dependencies gtowizardai)

add_executable(uniform_random_bot uniform_random_bot/uniform_random_bot.cpp)
target_link_libraries(uniform_random_bot dependencies gtowizardai)

add_executable(pokerbot main_bot/main.cpp)
target_link_libraries(pokerbot dependencies gtowizardai)

# Add tests
add_custom_target(tests)
file(GLOB_RECURSE tests RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS *_test.cpp)
foreach(test_file IN LISTS tests)
    get_filename_component(test_name ${test_file} NAME_WLE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} third_party gtest_main gmock_main gtowizardai)
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS tools)
    add_dependencies(tests ${test_name})
endforeach()

# Add scripts
add_subdirectory(scripts)
