#include "doctest/doctest.h"
#include "toy_sdl/deque.hpp"

#include <string>

TEST_SUITE("Deque modifiers") {
    TEST_CASE("Pushing back values should increase size") {
        SUBCASE("Pushing back one value") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_back(1);
            const auto size_after = d.size();
        
            CHECK(size_after > size_before);
        }

        SUBCASE("Pushing back multiple values") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_back(1);
            d.push_back(2);
            d.push_back(3);
            const auto size_after = d.size();
        
            CHECK(size_after == size_before + 3);
        }
    }

    TEST_CASE("Push back should work for different reference types") {
        SUBCASE("Const reference") {
            my::deque<int> d;
            d.push_back(static_cast<const int&>(1));
        }

        SUBCASE("Rvalue reference") {
            my::deque<int> d;
            d.push_back(static_cast<int&&>(1));
        }
    }

    TEST_CASE("Push back should add elements") {
        SUBCASE("Pushing back one value") {
            my::deque<int> d;

            d.push_back(1);
        
            CHECK(d[0] == 1);
        }

        SUBCASE("Pushing back multiple values") {
            my::deque<int> d;

            d.push_back(1);
            d.push_back(2);
            d.push_back(3);
        
            CHECK(d[0] == 1);
            CHECK(d[1] == 2);
            CHECK(d[2] == 3);
        }
    }

    TEST_CASE("Emplace back should increase size") {
        SUBCASE("Emplacing back one value") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.emplace_back(1);
            const auto size_after = d.size();
        
            CHECK(size_after > size_before);
        }

        SUBCASE("Emplacing back multiple values") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.emplace_back(1);
            d.emplace_back(2);
            d.emplace_back(3);
            const auto size_after = d.size();
        
            CHECK(size_after == size_before + 3);
        }
    }

    TEST_CASE("Emplace back should work with different constructors") {
        SUBCASE("Simple constructor") {
            my::deque<int> d;
            d.emplace_back(1);
        }

        SUBCASE("Nontrivial constructor") {
            my::deque<std::string> d;
            d.emplace_back("Hello");
        }

        SUBCASE("Constructor with multiple parameters") {
            struct Point { int x; int y; };
            my::deque<Point> d;

            const auto size_before = d.size();
            d.emplace_back(1, 2);
        }
    }

    TEST_CASE("Emplace back should add elements") {
        SUBCASE("Emplace back one value") {
            my::deque<int> d;

            d.emplace_back(1);
        
            CHECK(d[0] == 1);
        }

        SUBCASE("Emplace back multiple values") {
            my::deque<int> d;

            d.emplace_back(1);
            d.emplace_back(2);
            d.emplace_back(3);
        
            CHECK(d[0] == 1);
            CHECK(d[1] == 2);
            CHECK(d[2] == 3);
        }
    }


    TEST_CASE("Push front should increase size") {
        SUBCASE("Pushing one value") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_front(1);
            const auto size_after = d.size();
        
            CHECK(size_after > size_before);
        }

        SUBCASE("Pushing multiple values") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.push_front(1);
            d.push_front(2);
            d.push_front(3);
            const auto size_after = d.size();
        
            CHECK(size_after == size_before + 3);
        }
    }

    TEST_CASE("Push front should work for different reference types") {
        SUBCASE("Const reference") {
            my::deque<int> d;
            d.push_front(static_cast<const int&>(1));
        }

        SUBCASE("Rvalue reference") {
            my::deque<int> d;
            d.push_front(static_cast<int&&>(1));
        }
    }

    TEST_CASE("Push front should add elements") {
        SUBCASE("Pushing one value") {
            my::deque<int> d;

            d.push_front(1);
        
            CHECK(d[0] == 1);
        }

        SUBCASE("Pushing multiple values") {
            my::deque<int> d;

            d.push_front(1);
            d.push_front(2);
            d.push_front(3);
        
            CHECK(d[0] == 3);
            CHECK(d[1] == 2);
            CHECK(d[2] == 1);
        }
    }

    TEST_CASE("Emplace front should increase size") {
        SUBCASE("Emplacing one value") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.emplace_front(1);
            const auto size_after = d.size();
        
            CHECK(size_after > size_before);
        }

        SUBCASE("Emplacing multiple values") {
            my::deque<int> d;

            const auto size_before = d.size();
            d.emplace_front(1);
            d.emplace_front(2);
            d.emplace_front(3);
            const auto size_after = d.size();
        
            CHECK(size_after == size_before + 3);
        }
    }

    TEST_CASE("Emplace front should work with different constructors") {
        SUBCASE("Simple constructor") {
            my::deque<int> d;
            d.emplace_front(1);
        }

        SUBCASE("Nontrivial constructor") {
            my::deque<std::string> d;
            d.emplace_front("Hello");
        }

        SUBCASE("Constructor with multiple parameters") {
            struct Point { int x; int y; };
            my::deque<Point> d;

            const auto size_before = d.size();
            d.emplace_front(1, 2);
        }
    }

    TEST_CASE("Emplace front should add elements") {
        SUBCASE("Emplacing one value") {
            my::deque<int> d;

            d.emplace_front(1);
        
            CHECK(d[0] == 1);
        }

        SUBCASE("Emplacing multiple values") {
            my::deque<int> d;

            d.emplace_front(1);
            d.emplace_front(2);
            d.emplace_front(3);
        
            CHECK(d[0] == 3);
            CHECK(d[1] == 2);
            CHECK(d[2] == 1);
        }
    }

    TEST_CASE("Pop back should decrease size") {
        SUBCASE("Removing one element") {
            my::deque<int> deq;
            deq.push_back(1);
            deq.push_back(2);
            deq.push_back(3);

            const auto size_before = deq.size();
            deq.pop_back();
            const auto size_after = deq.size();

            CHECK(size_after < size_before);
        }

        SUBCASE("Removing multiple elements") {
            my::deque<int> deq;
            deq.push_back(1);
            deq.push_back(2);
            deq.push_back(3);
            deq.push_back(4);
            deq.push_back(5);

            const auto size_before = deq.size();
            deq.pop_back();
            deq.pop_back();
            deq.pop_back();
            const auto size_after = deq.size();

            CHECK(size_after == size_before - 3);
        }

        SUBCASE("Removing all elements") {
            my::deque<int> deq;
            deq.push_back(1);
            deq.push_back(2);
            deq.push_back(3);

            const auto number_of_elements = deq.size();
            for (std::size_t i = 0; i < number_of_elements; ++i) {
                deq.pop_back();
            }

            CHECK(deq.empty());
        }
    }

    TEST_CASE("Pop back should remove only back element") {
        my::deque<int> deq;
        deq.push_back(1);
        deq.push_back(2);
        deq.push_back(3);

        deq.pop_back();

        CHECK(deq[0] == 1);
        CHECK(deq[1] == 2);
    }

    TEST_CASE("Pop front should decrease size") {
        SUBCASE("Removing one element") {
            my::deque<int> deq;
            deq.push_back(1);
            deq.push_back(2);
            deq.push_back(3);

            const auto size_before = deq.size();
            deq.pop_front();
            const auto size_after = deq.size();

            CHECK(size_after < size_before);
        }

        SUBCASE("Removing multiple elements") {
            my::deque<int> deq;
            deq.push_back(1);
            deq.push_back(2);
            deq.push_back(3);
            deq.push_back(4);
            deq.push_back(5);

            const auto size_before = deq.size();
            deq.pop_front();
            deq.pop_front();
            deq.pop_front();
            const auto size_after = deq.size();

            CHECK(size_after == size_before - 3);
        }

        SUBCASE("Removing all elements") {
            my::deque<int> deq;
            deq.push_back(1);
            deq.push_back(2);
            deq.push_back(3);

            const auto number_of_elements = deq.size();
            for (std::size_t i = 0; i < number_of_elements; ++i) {
                deq.pop_front();
            }

            CHECK(deq.empty());
        }
    }

    TEST_CASE("Pop front should remove only front element") {
        my::deque<int> deq;
        deq.push_back(1);
        deq.push_back(2);
        deq.push_back(3);

        deq.pop_front();

        CHECK(deq[0] == 2);
        CHECK(deq[1] == 3);
    }

    TEST_CASE("Clear should remove all elements") {
        SUBCASE("Empty deque") {
            my::deque<int> deq;

            deq.clear();

            CHECK(deq.empty());
        }

        SUBCASE("Non-empty deque") {
            my::deque<int> deq;
            deq.push_back(1);
            deq.push_back(2);
            deq.push_back(3);
            
            deq.clear();

            CHECK(deq.empty());
        }
    }
}