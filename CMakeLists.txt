cmake_minimum_required(VERSION 3.27.0)
project(toy_stl LANGUAGES CXX)

add_library(toy_stl_lib INTERFACE)
target_compile_features(toy_stl_lib INTERFACE cxx_std_20)
target_include_directories(toy_stl_lib INTERFACE src)

add_executable(toy_stl_test
    tests/main.cpp
    tests/vector/vector.cpp
)

find_package(doctest CONFIG REQUIRED)
target_compile_features(toy_stl_test PRIVATE cxx_std_20)
target_link_libraries(toy_stl_test
    PRIVATE toy_stl_lib
    PRIVATE doctest::doctest
)

# This works but not reliable (need to refresh to trigger test discovery sometimes) and very slow, so i just use TestMate extension
enable_testing()
include(doctest)
doctest_discover_tests(toy_stl_test)