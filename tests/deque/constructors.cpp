#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

TEST_SUITE("Deque constructors") {
    TEST_CASE("Default constructor should create empty deque") {
        my::deque<int> d;
        
        CHECK(d.empty());
    }

    TEST_CASE("Constructor taking only allocator should create empty deque") {
        my::deque<int> d(std::allocator<int> {});
        
        CHECK(d.empty());
    }
}