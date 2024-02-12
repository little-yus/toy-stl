#include "doctest/doctest.h"
#include "my_std/vector.hpp"

TEST_CASE("Default construction") {
    my::vector<int> vec;
    
    CHECK(vec.empty());
}