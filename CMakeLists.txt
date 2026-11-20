cmake_minimum_required(VERSION 3.16)
project(mini_redis CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

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

# Tests
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

function(add_unit_test name)
  add_executable(${name} tests/${name}_test.cpp)
  target_link_libraries(${name} PRIVATE redis_core GTest::gtest_main)
  gtest_discover_tests(${name} TEST_PREFIX "${name}.")
endfunction()

add_unit_test(m1_store_commands)
add_unit_test(m2_resp_parser)
