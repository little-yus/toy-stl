cmake_minimum_required(VERSION 3.27.0)
project(standard_library LANGUAGES CXX)

enable_testing()

add_library(my_std_lib INTERFACE)
target_compile_features(my_std_lib INTERFACE cxx_std_20)
target_include_directories(my_std_lib INTERFACE src)

add_executable(my_std_lib_test
    tests/main.cpp
    tests/vector/vector.cpp
)

find_package(doctest CONFIG REQUIRED)
target_compile_features(my_std_lib_test PRIVATE cxx_std_20)
target_link_libraries(my_std_lib_test
    PRIVATE my_std_lib    
    PRIVATE doctest::doctest
)
add_test(NAME all_tests COMMAND my_std_lib_test)