#include "doctest/doctest.h"
#include "my_std/vector.hpp"

#include <iostream>

TEST_CASE("Default constructor") {
    my::vector<int> vec;
    
    CHECK(vec.empty());
}

TEST_CASE("Constructor from size") {
    SUBCASE("Zero size") {
        my::vector<int> vec(0);
    
        CHECK(vec.size() == 0);
    }
    
    SUBCASE("Nonzero size") {
        my::vector<int> vec(123);
    
        CHECK(vec.size() == 123);
    }

    SUBCASE("Zero initialized elements") {
        const std::size_t size = 10;
        my::vector<int> vec(size);

        bool all_elements_are_zero = true;
        for (int i = 0; i < size; i += 1) {
            if (vec[i] != int{0}) {
                all_elements_are_zero = false;
                break;
            }
        }
    
        CHECK(all_elements_are_zero);
    }
}

TEST_CASE("Move constructor") {
    my::vector<int> vec_a(3);
    vec_a[0] = 111;
    vec_a[1] = 222;
    vec_a[2] = 333;

    my::vector<int> vec_b(std::move(vec_a));
    
    CHECK(vec_b[0] == 111);
    CHECK(vec_b[1] == 222);
    CHECK(vec_b[2] == 333);
}

TEST_CASE("Constructor from size and value") {
    SUBCASE("Zero size") {
        my::vector<int> vec(0, 10);
    
        CHECK(vec.size() == 0);
    }
    
    SUBCASE("Nonzero size") {
        my::vector<int> vec(123, 10);
    
        CHECK(vec.size() == 123);
    }

    SUBCASE("Nonzero initialized elements") {
        const std::size_t size = 10;
        const int value = 123;

        my::vector<int> vec(size, value);

        bool all_elements_are_initialized = true;
        for (int i = 0; i < size; i += 1) {
            if (vec[i] != value) {
                all_elements_are_initialized = false;
                break;
            }
        }
    
        CHECK(all_elements_are_initialized);
    }
}

TEST_CASE("Subscript operator") {
    SUBCASE("Reading from default initialized vector") {
        my::vector<int> vec(10);
        CHECK(vec[0] == 0);
    }

    SUBCASE("Reading from value initialized vector") {
        my::vector<int> vec(10, 123);
        CHECK(vec[0] == 123);
    }

    SUBCASE("Writing at the same position") {
        my::vector<int> vec(10);

        vec[0] = 123;
        CHECK(vec[0] == 123);

        vec[0] = 456;
        CHECK(vec[0] == 456);
    }

    SUBCASE("Writing same value at different positions") {
        my::vector<int> vec(10);

        vec[0] = 123;
        CHECK(vec[0] == 123);

        vec[1] = 123;
        CHECK(vec[1] == 123);
    }

    SUBCASE("Writing different values at different positions") {
        my::vector<int> vec(10);

        vec[0] = 123;
        CHECK(vec[0] == 123);

        vec[1] = 456;
        CHECK(vec[0] == 123);
        CHECK(vec[1] == 456);
    }
}