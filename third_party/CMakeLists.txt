# Update submodules
# NB - Stopped using submodules to speedup the build time
# message(STATUS "Updating git submodules")
# execute_process(COMMAND git submodule update --init --recursive)


# Add subdirectories
foreach(subdirectory IN ITEMS fmt googletest)
    add_subdirectory(${subdirectory})
endforeach()

# GoogleTest
include(GoogleTest)

# Export interface library
add_library(third_party INTERFACE)
# target_include_directories(third_party INTERFACE X)
target_link_libraries(third_party INTERFACE fmt::fmt)
