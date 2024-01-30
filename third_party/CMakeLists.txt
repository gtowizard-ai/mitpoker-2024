# Update submodules
# NB - Stopped using submodules to speedup the build time
# message(STATUS "Updating git submodules")
# execute_process(COMMAND git submodule update --init --recursive)

# fmt
add_subdirectory(fmt)

# GoogleTest
if(NOT IS_BUILDING_BOT)
    add_subdirectory(googletest)
    include(GoogleTest)
endif()

# Export interface library
add_library(third_party INTERFACE)
target_include_directories(third_party INTERFACE pdqsort)
target_link_libraries(third_party INTERFACE fmt::fmt)
