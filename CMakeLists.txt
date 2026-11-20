cmake_minimum_required(VERSION 3.16)
project(mini_redis CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # lets your editor understand the project

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# ---------------------------------------------------------------------------
# Core library: all the real logic. The server binary and the tests both link
# against it, so everything you write is testable without a network.
# Add new .cpp files here as the project grows.
# ---------------------------------------------------------------------------
add_library(redis_core
  src/store.cpp
  src/resp.cpp
  src/commands.cpp
)
target_include_directories(redis_core PUBLIC src)
target_compile_options(redis_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(mini-redis src/main.cpp)
target_link_libraries(mini-redis PRIVATE redis_core)
target_compile_options(mini-redis PRIVATE -Wall -Wextra -Wpedantic)

# ---------------------------------------------------------------------------
# Tests (GoogleTest, downloaded automatically on first configure).
# One test binary per milestone, so you only see failures for what you're
# working on: `ctest --test-dir build -R m1 --output-on-failure`
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

function(add_milestone_test name)
  add_executable(${name} tests/${name}_test.cpp)
  target_link_libraries(${name} PRIVATE redis_core GTest::gtest_main)
  gtest_discover_tests(${name} TEST_PREFIX "${name}.")
endfunction()

add_milestone_test(m1_store_commands)
add_milestone_test(m2_resp_parser)
