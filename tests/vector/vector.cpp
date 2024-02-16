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

TEST_CASE("Copy constructor") {
    my::vector<int> vec_a(3);
    vec_a[0] = 111;
    vec_a[1] = 222;
    vec_a[2] = 333;

    my::vector<int> vec_b(vec_a);

    CHECK(vec_b.size() == vec_a.size());
    
    CHECK(vec_b[0] == 111);
    CHECK(vec_b[1] == 222);
    CHECK(vec_b[2] == 333);
}

TEST_CASE("Move constructor") {
    SUBCASE("Primitive type") {
        my::vector<int> vec_a(3);
        vec_a[0] = 111;
        vec_a[1] = 222;
        vec_a[2] = 333;

        my::vector<int> vec_b(std::move(vec_a));
        
        CHECK(vec_b[0] == 111);
        CHECK(vec_b[1] == 222);
        CHECK(vec_b[2] == 333);
    }

    SUBCASE("Move only type") {
        class MoveOnly
        {
        public:
            MoveOnly() {};
            MoveOnly(int value) : value(value) {}

            MoveOnly(const MoveOnly&) = delete;
            MoveOnly& operator= (const MoveOnly&) = delete;

            MoveOnly(MoveOnly&&) = default;
            MoveOnly& operator= (MoveOnly&&) = default;

            int value{0};
        };

        my::vector<MoveOnly> vec_a(3);
        vec_a[0] = 111;
        vec_a[1] = 222;
        vec_a[2] = 333;

        my::vector<MoveOnly> vec_b(std::move(vec_a));
        
        CHECK(vec_b[0].value == 111);
        CHECK(vec_b[1].value == 222);
        CHECK(vec_b[2].value == 333);
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

TEST_CASE("Subscript with bounds checking") {
    SUBCASE("Reading from default initialized vector") {
        my::vector<int> vec(10);
        CHECK(vec.at(0) == 0);
    }

    SUBCASE("Reading from value initialized vector") {
        my::vector<int> vec(10, 123);
        CHECK(vec.at(0) == 123);
    }

    SUBCASE("Writing at the same position") {
        my::vector<int> vec(10);

        vec.at(0) = 111;
        CHECK(vec.at(0) == 111);

        vec.at(0) = 222;
        CHECK(vec.at(0) == 222);
    }

    SUBCASE("Writing same value at different positions") {
        my::vector<int> vec(10);

        vec.at(0) = 123;
        CHECK(vec.at(0) == 123);

        vec.at(1) = 123;
        CHECK(vec.at(1) == 123);
    }

    SUBCASE("Writing different values at different positions") {
        my::vector<int> vec(10);

        vec.at(0) = 111;
        CHECK(vec.at(0) == 111);

        vec.at(1) = 222;
        CHECK(vec.at(0) == 111);
        CHECK(vec.at(1) == 222);
    }

    SUBCASE("Writing different values at different positions") {
        const std::size_t size = 10;
        my::vector<int> vec(size);

        CHECK_THROWS_AS(vec.at(size + 1), std::out_of_range);
        CHECK_THROWS_AS(vec.at(-1), std::out_of_range);
    }
}

TEST_CASE("Front function") {
    SUBCASE("Reading") {
        my::vector<int> vec(10);
        vec[0] = 123;

        CHECK(std::as_const(vec).front() == 123);
    }

    SUBCASE("Writing") {
        my::vector<int> vec(10);
        vec.front() = 123;

        CHECK(vec[0] == 123);
    }
}

TEST_CASE("Back function") {
    const std::size_t size = 10;

    SUBCASE("Reading") {
        my::vector<int> vec(size);
        vec[size - 1] = 123;

        CHECK(std::as_const(vec).back() == 123);
    }

    SUBCASE("Writing") {
        my::vector<int> vec(size);
        vec.back() = 123;

        CHECK(vec[size - 1] == 123);
    }
}

TEST_CASE("pop_back") {
    my::vector<int> vec(10);
    vec.pop_back();

    CHECK(vec.size() == 9);
}

TEST_CASE("Adding elements to the back") {
    SUBCASE("Adding element increases size") {
        const std::size_t size = 5;
        my::vector<int> vec(size);
        vec.push_back(123);
        
        CHECK(vec.size() == size + 1);
        CHECK(vec.capacity() >= size + 1);
    }

    SUBCASE("Element can be accessed after addition") {
        const std::size_t size = 5;
        my::vector<int> vec(size);

        vec.push_back(123);

        CHECK(vec.back() == 123);
    }

    SUBCASE("Adding then removing element preserves previous size") {
        const std::size_t size = 5;
        my::vector<int> vec(size);
        REQUIRE(vec.size() == size);
        REQUIRE(vec.capacity() >= size);

        vec.push_back(123);
        vec.pop_back();
        
        CHECK(vec.size() == size);
        CHECK(vec.capacity() >= size);
    }
}

TEST_CASE("Vector clear") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() >= 0);

        vec.clear();

        CHECK(vec.empty());
        CHECK(vec.capacity() >= 0);
    }

    SUBCASE("Non-empty vector") {
        const std::size_t size = 5;
        my::vector<int> vec(5);
        REQUIRE(vec.size() == size);
        REQUIRE(vec.capacity() >= size);

        vec.clear();

        CHECK(vec.empty());
        // capacity should not be changed on clear
        // https://stackoverflow.com/questions/18467624/what-does-the-standard-say-about-how-calling-clear-on-a-vector-changes-the-capac/18467916#18467916
        CHECK(vec.capacity() >= size);
    }
}

TEST_CASE("Vector resize") {
    const std::size_t initial_size = 5;
    my::vector<int> vec(initial_size);
    REQUIRE(vec.size() == initial_size);

    SUBCASE("Resizing to same size does nothing") {
        // Doesn't really test much
        // Should check if any construction or moving happens using some custom type instead of int
        const std::size_t new_size = initial_size;
        vec.resize(new_size);
        CHECK(vec.size() == initial_size);
    }

    SUBCASE("Resizing down decreases size") {
        const std::size_t new_size = 3;
        vec.resize(new_size);
        CHECK(vec.size() == new_size);
    }

    SUBCASE("Resizing down does not change capacity") {
        const std::size_t new_size = 3;
        const std::size_t capacity_before_resize = vec.capacity();
        vec.resize(new_size);
        CHECK(vec.capacity() == capacity_before_resize);
    }

    // TODO: Resizing down destroys elements

    SUBCASE("Resizing up inserts default constructed elements") {
        const std::size_t new_size = 10;
        vec.resize(new_size);

        REQUIRE(vec.size() == new_size);
        for (int i = initial_size; i < new_size; i += 1) {
            CHECK(vec[i] == int{});
        }
    }

    SUBCASE("When resizing up you can insert copies of specified value") {
        const std::size_t new_size = 10;
        const int new_value = 123;
        vec.resize(new_size, new_value);

        REQUIRE(vec.size() == new_size);
        for (int i = initial_size; i < new_size; i += 1) {
            CHECK(vec[i] == new_value);
        }
    }
}

TEST_CASE("Vector capacity reserve") {
    SUBCASE("Reserve cannot reduce capacity") {
        const std::size_t size = 10;
        my::vector<int> vec(size);
        REQUIRE(vec.capacity() >= size);

        const std::size_t capacity_before_reserve = vec.capacity();
        vec.reserve(capacity_before_reserve - 3);

        CHECK(vec.capacity() == capacity_before_reserve);
    }

    SUBCASE("Reserve does nothing if requested capacity equals to current capacity") {
        const std::size_t size = 10;
        my::vector<int> vec(size);
        REQUIRE(vec.capacity() >= size);

        const std::size_t capacity_before_reserve = vec.capacity();
        vec.reserve(capacity_before_reserve);

        CHECK(vec.capacity() == capacity_before_reserve);
    }

    SUBCASE("Reserve does nothing if requested capacity equals to current capacity") {
        const std::size_t size = 10;
        my::vector<int> vec(size);
        REQUIRE(vec.capacity() >= size);

        const std::size_t capacity_before_reserve = vec.capacity();
        const std::size_t requested_capacity = capacity_before_reserve + 10;
        vec.reserve(requested_capacity);

        CHECK(vec.capacity() >= requested_capacity);
    }

    SUBCASE("Reserving does not change size") {
        my::vector<int> vec(10);
        const std::size_t size_before_reserve = vec.size();

        vec.reserve(vec.capacity() + 10);

        CHECK(vec.size() == size_before_reserve);
    }
}

TEST_CASE("Vector capacity shrink") {
    my::vector<int> vec;
    vec.reserve(10);
    vec.resize(5);
    REQUIRE(vec.size() == 5);
    REQUIRE(vec.capacity() >= 10);

    vec.shrink_to_fit();

    CHECK(vec.size() == 5);
    CHECK(vec.capacity() == vec.size()); // Not necessary in the standard
}