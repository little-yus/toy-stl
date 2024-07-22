#include "doctest/doctest.h"
#include "toy_stl/deque.hpp"

TEST_SUITE("Deque comparison operators") {
    TEST_CASE("Equality should work properly for different sizes") {
        SUBCASE("Both vectors empty") {
            my::deque<int> a;
            my::deque<int> b;
            REQUIRE(a.empty());
            REQUIRE(b.empty());

            CHECK(a == b);
        }

        SUBCASE("Different sizes") {
            my::deque<int> a;
            my::deque<int> b = {1, 2, 3};
            REQUIRE(a.size() != b.size());

            CHECK_FALSE(a == b);
        }

        SUBCASE("Same sizes, different elements") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 4, 5, 6 };
            REQUIRE(a.size() == b.size());

            CHECK_FALSE(a == b);
        }

        SUBCASE("Same sizes, same elements") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 1, 2, 3 };

            CHECK(a == b);
        }
    }

    TEST_CASE("Equality should satisfy properties of equivalence relation") {
        SUBCASE("Reflexivity") {
            my::deque<int> a = { 1, 2, 3 };

            CHECK(a == a);
        }

        SUBCASE("Symmetry") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 1, 2, 3 };

            REQUIRE(a == b);
            CHECK(b == a);
        }

        SUBCASE("Transitivity") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 1, 2, 3 };
            my::deque<int> c = { 1, 2, 3 };

            REQUIRE(a == b);
            REQUIRE(b == c);
            CHECK(a == c);
        }
    }

    TEST_CASE("Relational operators should work correctly for different sizes") {
        SUBCASE("Both vectors are empty") {
            my::deque<int> a;
            my::deque<int> b;
            REQUIRE(a.empty());
            REQUIRE(b.empty());
            
            CHECK_FALSE(a < b);
            CHECK_FALSE(b > a);
        }

        SUBCASE("One vector is empty") {
            my::deque<int> a;
            my::deque<int> b { 1, 2, 3 };
            REQUIRE(a.size() == 0);
            
            CHECK(a < b);
            CHECK(b > a);
        }

        SUBCASE("Same size, common prefix") {
            my::deque<int> a { 1, 1 };
            my::deque<int> b { 1, 2 };
            REQUIRE(a.size() == b.size());
            
            CHECK(a < b);
            CHECK(b > a);
        }

        SUBCASE("Same size, no common prefix") {
            my::deque<int> a { 1, 1 };
            my::deque<int> b { 2, 2 };
            REQUIRE(a.size() == b.size());
            
            CHECK(a < b);
            CHECK(b > a);
        }

        SUBCASE("Different size, one is prefix of other") {
            my::deque<int> a { 1 };
            my::deque<int> b { 1, 2, 3 };
            REQUIRE(a.size() != b.size());
            
            CHECK(a < b);
            CHECK(b > a);
        }

        SUBCASE("Different size, not prefixes of each other") {
            my::deque<int> a { 1, 2, 3 };
            my::deque<int> b { 2 };
            REQUIRE(a.size() != b.size());
            
            CHECK(a < b);
            CHECK(b > a);
        }
    }

    TEST_CASE("Relational operators should satisfy requirements of strict partial order")
    {
        SUBCASE("Antireflexivity") {
            my::deque<int> a { 1, 2, 3 };

            CHECK_FALSE(a < a);
        }

        SUBCASE("Asymmetry") {
            my::deque<int> a { 1, 2, 3 };
            my::deque<int> b { 2, 3, 4 };

            REQUIRE(a < b);
            CHECK_FALSE(b < a);
        }

        SUBCASE("Transitivity") {
            my::deque<int> a { 1, 2, 3 };
            my::deque<int> b { 2, 3, 4 };
            my::deque<int> c { 3, 4, 5 };

            REQUIRE(a < b);
            REQUIRE(b < c);
            CHECK(a < c);
        }
    }

    TEST_CASE("Spaceship operator shoudld work with strongly ordered elements") {
        SUBCASE("Less") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 2, 3, 4 };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::strong_ordering>);
            CHECK((a <=> b) == std::strong_ordering::less);
        }

        SUBCASE("Greater") {
            my::deque<int> a = { 1, 2, 100 };
            my::deque<int> b = { 1, 2, 3, 4 };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::strong_ordering>);
            CHECK((a <=> b) == std::strong_ordering::greater);
        }

        SUBCASE("Equal") {
            my::deque<int> a = { 1, 2, 3 };
            my::deque<int> b = { 1, 2, 3 };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::strong_ordering>);
            CHECK((a <=> b) == std::strong_ordering::equal);
        }
    }

    TEST_CASE("Spaceship operator should work with weakly ordered elements") {
        struct WeaklyOrdered
        {
            bool operator==(const WeaklyOrdered& other) const
            {
                return x == other.x;
            }

            std::weak_ordering operator<=>(const WeaklyOrdered& other) const
            {
                return std::weak_order(x, other.x);
            }

            int x;
            int y;
        };
        static_assert(std::three_way_comparable<WeaklyOrdered, std::weak_ordering>);

        SUBCASE("Less") {
            my::deque<WeaklyOrdered> a = { WeaklyOrdered { .x = 1, .y = 0 } };
            my::deque<WeaklyOrdered> b = { WeaklyOrdered { .x = 2, .y = 0 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::weak_ordering>);
            CHECK((a <=> b) == std::weak_ordering::less);
        }

        SUBCASE("Greater") {
            my::deque<WeaklyOrdered> a = { WeaklyOrdered { .x = 2, .y = 0 } };
            my::deque<WeaklyOrdered> b = { WeaklyOrdered { .x = 1, .y = 0 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::weak_ordering>);
            CHECK((a <=> b) == std::weak_ordering::greater);
        }

        SUBCASE("Equivalent, all fields equal") {
            my::deque<WeaklyOrdered> a = { WeaklyOrdered { .x = 1, .y = 2 } };
            my::deque<WeaklyOrdered> b = { WeaklyOrdered { .x = 1, .y = 2 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::weak_ordering>);
            CHECK((a <=> b) == std::weak_ordering::equivalent);
        }

        SUBCASE("Equivalent, not all fields equal") {
            my::deque<WeaklyOrdered> a = { WeaklyOrdered { .x = 1, .y = 2 } };
            my::deque<WeaklyOrdered> b = { WeaklyOrdered { .x = 1, .y = 3 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::weak_ordering>);
            CHECK((a <=> b) == std::weak_ordering::equivalent);
        }
    }

    TEST_CASE("Spaceship operator should work with partially ordered elements") {
        struct PartiallyOrdered
        {
            bool operator==(const PartiallyOrdered& other) const
            {
                return x == other.x && y == other.y;
            }

            std::partial_ordering operator<=>(const PartiallyOrdered& other) const
            {
                if (x == other.x && y == other.y) {
                    return std::partial_ordering::equivalent;
                }

                if (x < other.x && y < other.y) {
                    return std::partial_ordering::less;
                }

                if (x > other.x && y > other.y) {
                    return std::partial_ordering::greater;
                }

                return std::partial_ordering::unordered;
            }

            int x;
            int y;
        };
        static_assert(std::three_way_comparable<PartiallyOrdered, std::partial_ordering>);

        SUBCASE("Less") {
            my::deque<PartiallyOrdered> a { PartiallyOrdered { .x = 1, .y = 1 } };
            my::deque<PartiallyOrdered> b { PartiallyOrdered { .x = 2, .y = 2 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::partial_ordering>);
            CHECK((a <=> b) == std::partial_ordering::less);
        }

        SUBCASE("Greater") {
            my::deque<PartiallyOrdered> a { PartiallyOrdered { .x = 3, .y = 5 } };
            my::deque<PartiallyOrdered> b { PartiallyOrdered { .x = 2, .y = 2 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::partial_ordering>);
            CHECK((a <=> b) == std::partial_ordering::greater);
        }

        SUBCASE("Equivalent") {
            my::deque<PartiallyOrdered> a { PartiallyOrdered { .x = 1, .y = 1 } };
            my::deque<PartiallyOrdered> b { PartiallyOrdered { .x = 1, .y = 1 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::partial_ordering>);
            CHECK((a <=> b) == std::partial_ordering::equivalent);
        }

        SUBCASE("Unordered") {
            my::deque<PartiallyOrdered> a { PartiallyOrdered { .x = 1, .y = 2 } };
            my::deque<PartiallyOrdered> b { PartiallyOrdered { .x = 2, .y = 1 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::partial_ordering>);
            CHECK((a <=> b) == std::partial_ordering::unordered);
        }
    }

    TEST_CASE("Spaceship operator should work with classes with operator<") {
        struct WithoutSpaceship
        {
            bool operator<(const WithoutSpaceship& other) const
            {
                return x < other.x;
            }

            int x;
        };
        static_assert(!std::three_way_comparable<WithoutSpaceship>);

        SUBCASE("Equal") {
            my::deque<WithoutSpaceship> a { WithoutSpaceship { .x = 1 } };
            my::deque<WithoutSpaceship> b { WithoutSpaceship { .x = 1 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b)>);
            CHECK((a <=> b) == std::weak_ordering::equivalent);
        }

        SUBCASE("Less") {
            my::deque<WithoutSpaceship> a { WithoutSpaceship { .x = 1 } };
            my::deque<WithoutSpaceship> b { WithoutSpaceship { .x = 2 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b)>);
            CHECK((a <=> b) == std::weak_ordering::less);
        }

        SUBCASE("Greater") {
            my::deque<WithoutSpaceship> a { WithoutSpaceship { .x = 2 } };
            my::deque<WithoutSpaceship> b { WithoutSpaceship { .x = 1 } };

            REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b)>);
            CHECK((a <=> b) == std::weak_ordering::greater);
        }
    }
}