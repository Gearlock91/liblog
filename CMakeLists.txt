# CMakeList.txt : CMake project for Logger, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 4.1)
project (Logger)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

add_library(Logger STATIC src/Log.cpp)
target_include_directories(Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(logger_tests tests/logger_tests.cpp)
target_link_libraries(logger_tests PRIVATE Logger GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(logger_tests)