#include "doctest/doctest.h"
#include "my_std/vector.hpp"

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
}