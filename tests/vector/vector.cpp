#include "doctest/doctest.h"
#include "toy_stl/vector.hpp"

#include <iterator>
#include <ranges>
#include <memory_resource>

#include <sstream>

namespace doctest
{
template <typename T>
struct StringMaker<my::vector<T>>
{
    static String convert(const my::vector<T>& v) {
        std::ostringstream oss;

        oss << "[";

        if (!v.empty()) { // !v.empty() does not work!!??
            oss << v.front();
            for(auto it = v.cbegin() + 1; it != v.cend(); ++it) {
                oss << ", " << *it;
            }
        }
        
        oss << "]";

        return oss.str().c_str();
    }
};
}

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

TEST_CASE("Constructing from iterator pair") {
    SUBCASE("Copying empty vector") {
        my::vector<int> vec_a;
        REQUIRE(vec_a.empty());

        my::vector<int> vec_b(vec_a.begin(), vec_a.end());

        CHECK(vec_b.empty());
    }

    SUBCASE("Copying vector") {
        my::vector<int> vec_a(3);
        vec_a[0] = 111;
        vec_a[1] = 222;
        vec_a[2] = 333;

        my::vector<int> vec_b(vec_a.begin(), vec_a.end());

        CHECK(vec_a.size() == vec_b.size());
        for (std::size_t i = 0; i < vec_a.size(); i += 1) {
            CHECK(vec_a[i] == vec_b[i]);
        }
    }

    SUBCASE("Copying vector in reverse") {
        my::vector<int> vec_a(3);
        vec_a[0] = 111;
        vec_a[1] = 222;
        vec_a[2] = 333;

        my::vector<int> vec_b(vec_a.rbegin(), vec_a.rend());

        CHECK(vec_a.size() == vec_b.size());
        for (
            std::size_t i_a = 0, i_b = vec_b.size() - 1;
            i_a < vec_a.size();
            i_a++, i_b--
        ) {
            CHECK(vec_a[i_a] == vec_b[i_b]);
        }
    }
}

TEST_CASE("Constructing from initializer_list") {
    SUBCASE("Explicit initializer_list") {
        std::initializer_list<int> init_list { 1, 2, 3 };

        my::vector<int> vec(init_list);

        CHECK(vec.size() == init_list.size());
        CHECK(vec[0] == 1);
        CHECK(vec[1] == 2);
        CHECK(vec[2] == 3);
    }

    SUBCASE("Implicit initializer_list") {
        my::vector<int> vec = { 1, 2, 3 };

        CHECK(vec.size() == 3);
        CHECK(vec[0] == 1);
        CHECK(vec[1] == 2);
        CHECK(vec[2] == 3);
    }

    SUBCASE("Assigning initializer_list") {
        my::vector<int> vec(10);
        vec = { 1, 2, 3 };

        CHECK(vec.size() == 3);
        CHECK(vec[0] == 1);
        CHECK(vec[1] == 2);
        CHECK(vec[2] == 3);
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

TEST_CASE("Constructing new element in-place at the end") {
    SUBCASE("Adding element increases size") {
        const std::size_t size = 5;
        my::vector<int> vec(size);
        vec.emplace_back(123);
        
        CHECK(vec.size() == size + 1);
        CHECK(vec.capacity() >= size + 1);
    }

    SUBCASE("Element can be accessed after addition") {
        const std::size_t size = 5;
        my::vector<int> vec(size);

        vec.emplace_back(123);

        CHECK(vec.back() == 123);
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

TEST_CASE("Vector equality") {
    SUBCASE("Both vectors empty") {
        my::vector<int> a;
        my::vector<int> b;
        REQUIRE(a.empty());
        REQUIRE(b.empty());

        CHECK(a == b);
    }

    SUBCASE("Different size") {
        my::vector<int> a;
        my::vector<int> b {1, 2, 3};
        REQUIRE(a.size() != b.size());

        CHECK_FALSE(a == b);
    }

    SUBCASE("Same size, different elements") {
        my::vector<int> a { 1, 2, 3 };
        my::vector<int> b { 4, 5, 6 };
        REQUIRE(a.size() == b.size());

        CHECK_FALSE(a == b);
    }

    SUBCASE("Same size, same elements") {
        my::vector<int> a { 1, 2, 3 };
        my::vector<int> b { 1, 2, 3 };

        CHECK(a == b);
    }

    // Would be nice if these were randomized
    SUBCASE("Reflexivity") {
        my::vector<int> a { 1, 2, 3 };

        CHECK(a == a);
    }

    SUBCASE("Symmetry") {
        my::vector<int> a { 1, 2, 3 };
        my::vector<int> b { 1, 2, 3 };

        REQUIRE(a == b);
        CHECK(b == a);
    }

    SUBCASE("Transitivity") {
        my::vector<int> a { 1, 2, 3 };
        my::vector<int> b { 1, 2, 3 };
        my::vector<int> c { 1, 2, 3 };

        REQUIRE(a == b);
        REQUIRE(b == c);
        CHECK(a == c);
    }
}

TEST_CASE("Vector ordering") {
    SUBCASE("Both vectors are empty") {
        my::vector<int> a;
        my::vector<int> b;
        REQUIRE(a.size() == 0);
        
        CHECK_FALSE(a < b);
        CHECK_FALSE(b > a);
    }

    SUBCASE("One vector is empty") {
        my::vector<int> a;
        my::vector<int> b { 1, 2, 3 };
        REQUIRE(a.size() == 0);
        
        CHECK(a < b);
        CHECK(b > a);
    }

    SUBCASE("Same size, common prefix") {
        my::vector<int> a { 1, 1 };
        my::vector<int> b { 1, 2 };
        REQUIRE(a.size() == b.size());
        
        CHECK(a < b);
        CHECK(b > a);
    }

    SUBCASE("Same size, no common prefix") {
        my::vector<int> a { 1, 1 };
        my::vector<int> b { 2, 2 };
        REQUIRE(a.size() == b.size());
        
        CHECK(a < b);
        CHECK(b > a);
    }

    SUBCASE("Different size, one is prefix of other") {
        my::vector<int> a { 1 };
        my::vector<int> b { 1, 2, 3 };
        REQUIRE(a.size() != b.size());
        
        CHECK(a < b);
        CHECK(b > a);
    }

    SUBCASE("Different size, not prefixes of each other") {
        my::vector<int> a { 1, 2, 3 };
        my::vector<int> b { 2 };
        REQUIRE(a.size() != b.size());
        
        CHECK(a < b);
        CHECK(b > a);
    }

    SUBCASE("Antireflexivity") {
        my::vector<int> a { 1, 2, 3 };

        CHECK_FALSE(a < a);
    }

    SUBCASE("Assymetry") {
        my::vector<int> a { 1, 2, 3 };
        my::vector<int> b { 2, 3, 4 };

        REQUIRE(a < b);
        CHECK_FALSE(b < a);
    }

    SUBCASE("Transitivity") {
        my::vector<int> a { 1, 2, 3 };
        my::vector<int> b { 2, 3, 4 };
        my::vector<int> c { 3, 4, 5 };

        REQUIRE(a < b);
        REQUIRE(b < c);
        CHECK(a < c);
    }
}

TEST_CASE("Vector spaceship operator") {
    SUBCASE("Strong order") {
        my::vector<int> a { 1, 2, 3 };
        my::vector<int> b { 2, 3, 4 };

        REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::strong_ordering>);
        CHECK((a <=> b) == std::strong_ordering::less);
    }

    struct WeaklyOrderedPair
    {
        bool operator== (const WeaklyOrderedPair& other) const
        {
            return x == other.x;
        }

        std::weak_ordering operator<=> (const WeaklyOrderedPair& other) const
        {
            return std::weak_order(x, other.x);
        }

        int x;
        int y;
    };
    static_assert(std::three_way_comparable<WeaklyOrderedPair, std::weak_ordering>);

    SUBCASE("Weak order less") {
        my::vector<WeaklyOrderedPair> a { WeaklyOrderedPair { .x = 1, .y = 0 } };
        my::vector<WeaklyOrderedPair> b { WeaklyOrderedPair { .x = 2, .y = 0 } };

        REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::weak_ordering>);
        CHECK((a <=> b) == std::weak_ordering::less);
    }

    SUBCASE("Weak order equivalent") {
        my::vector<WeaklyOrderedPair> a { WeaklyOrderedPair { .x = 0, .y = 1 } };
        my::vector<WeaklyOrderedPair> b { WeaklyOrderedPair { .x = 0, .y = 2 } };

        REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::weak_ordering>);
        CHECK((a <=> b) == std::weak_ordering::equivalent);
    }

    struct PartiallyOrderedPoint
    {
        bool operator== (const PartiallyOrderedPoint& other) const
        {
            return x == other.x && y == other.y;
        }

        std::partial_ordering operator<=> (const PartiallyOrderedPoint& other) const
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
    static_assert(std::three_way_comparable<PartiallyOrderedPoint, std::partial_ordering>);

    SUBCASE("Partial order less") {
        my::vector<PartiallyOrderedPoint> a { PartiallyOrderedPoint { .x = 1, .y = 1 } };
        my::vector<PartiallyOrderedPoint> b { PartiallyOrderedPoint { .x = 2, .y = 2 } };

        REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::partial_ordering>);
        CHECK((a <=> b) == std::partial_ordering::less);
    }

    SUBCASE("Partial order equivalent") {
        my::vector<PartiallyOrderedPoint> a { PartiallyOrderedPoint { .x = 1, .y = 1 } };
        my::vector<PartiallyOrderedPoint> b { PartiallyOrderedPoint { .x = 1, .y = 1 } };

        REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::partial_ordering>);
        CHECK((a <=> b) == std::partial_ordering::equivalent);
    }

    SUBCASE("Partial order unordered") {
        my::vector<PartiallyOrderedPoint> a { PartiallyOrderedPoint { .x = 1, .y = 2 } };
        my::vector<PartiallyOrderedPoint> b { PartiallyOrderedPoint { .x = 2, .y = 1 } };

        REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b), std::partial_ordering>);
        CHECK((a <=> b) == std::partial_ordering::unordered);
    }

    struct PointWithoutSpaceship
    {
        bool operator== (const PointWithoutSpaceship& other) const
        {
            return x == other.x;
        }

        bool operator< (const PointWithoutSpaceship& other) const
        {
            return x < other.x;
        }

        int x;
    };
    static_assert(!std::three_way_comparable<PointWithoutSpaceship>);

    SUBCASE("Syntesized order equal") {
        my::vector<PointWithoutSpaceship> a { PointWithoutSpaceship { .x = 1 } };
        my::vector<PointWithoutSpaceship> b { PointWithoutSpaceship { .x = 1 } };

        REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b)>);
        CHECK((a <=> b) == std::weak_ordering::equivalent);
    }

    SUBCASE("Syntesized order less") {
        my::vector<PointWithoutSpaceship> a { PointWithoutSpaceship { .x = 1 } };
        my::vector<PointWithoutSpaceship> b { PointWithoutSpaceship { .x = 2 } };

        REQUIRE(std::three_way_comparable_with<decltype(a), decltype(b)>);
        CHECK((a <=> b) == std::weak_ordering::less);
    }
}

// TODO: Add test cases covering insertion with reallocation
TEST_CASE("Ordinary insertion") {
    SUBCASE("Insert before first element") {
        my::vector<int> vec = { 1, 2, 3 };
        vec.insert(vec.cbegin(), static_cast<const int&>(0));
        CHECK(vec == my::vector { 0, 1, 2, 3 });
    }

    SUBCASE("Insert before end element (same as push back)") {
        my::vector<int> a = { 1, 2, 3 };
        my::vector<int> b = { 1, 2, 3 };

        a.insert(a.cend(), static_cast<const int&>(4));
        b.push_back(4);

        CHECK(a == b);
    }

    SUBCASE("Insert in the middle") {
        my::vector<int> vec = { 1, 2, 3 };
        vec.insert(vec.cbegin() + 2, static_cast<const int&>(123));
        CHECK(vec == my::vector { 1, 2, 123, 3 });
    }
}

TEST_CASE("Rvalue reference insertion") {
    SUBCASE("Insert before first element") {
        my::vector<int> vec = { 1, 2, 3 };
        vec.insert(vec.cbegin(), std::move(0));
        CHECK(vec == my::vector { 0, 1, 2, 3 });
    }

    SUBCASE("Insert before end element (same as push back)") {
        my::vector<int> a = { 1, 2, 3 };
        my::vector<int> b = { 1, 2, 3 };

        a.insert(a.cend(), std::move(4));
        b.push_back(std::move(4));

        CHECK(a == b);
    }

    SUBCASE("Insert in the middle") {
        my::vector<int> vec = { 1, 2, 3 };
        vec.insert(vec.cbegin() + 2, std::move(123));
        CHECK(vec == my::vector { 1, 2, 123, 3 });
    }
}

TEST_CASE("Inserting copies") {
    SUBCASE("Inserting zero copies does nothing") {
        my::vector<int> vec = { 1, 2, 3 };
        
        vec.insert(vec.cend(), 0, 123);
        vec.insert(vec.cbegin(), 0, 123);
        vec.insert(vec.cbegin() + 1, 0, 123);

        CHECK(vec == my::vector { 1, 2, 3 });
    }

    SUBCASE("Inserting before begin and end has the same effect for empty vector") {
        my::vector<int> a;
        my::vector<int> b;

        a.insert(a.cbegin(), 3, 123);
        b.insert(b.cend(), 3, 123);

        CHECK(a == b);
        CHECK(a == my::vector { 123, 123, 123 });
    }

    SUBCASE("Inserting before the end has is the same as push back") {
        my::vector<int> a = { 1, 2, 3 };
        my::vector<int> b = { 1, 2, 3 };

        a.insert(a.cend(), 3, 123);
        b.push_back(123);
        b.push_back(123);
        b.push_back(123);

        CHECK(a == b);
        CHECK(a == my::vector { 1, 2, 3, 123, 123, 123 });
    }

    SUBCASE("Insertion moves elements after insert position forward") {
        my::vector<int> a = { 1, 2, 3, 4 };

        a.insert(a.cbegin() + 2, 3, 123);

        CHECK(a == my::vector { 1, 2, 123, 123, 123, 3, 4 });
    }
}

TEST_CASE("Inserting range") {
    SUBCASE("Inserting empty range does nothing") {
        my::vector<int> a = { 1, 2, 3 };
        my::vector<int> b = { 4, 5, 6 };
        
        a.insert(a.cbegin(), b.cbegin(), b.cbegin());
        a.insert(a.cend(), b.cend(), b.cend());

        CHECK(a == my::vector { 1, 2, 3 });
    }

    SUBCASE("Inserting before begin and end has the same effect for empty vector") {
        my::vector<int> a = { 1, 2, 3 };
        my::vector<int> b;
        my::vector<int> c;

        b.insert(b.cbegin(), a.cbegin(), a.cend());
        c.insert(c.cend(), a.cbegin(), a.cend());

        CHECK(b == c);
        CHECK(c == a);
    }

    SUBCASE("Inserting before the end has is the same as push back") {
        my::vector<int> a = { 1, 2, 3 };
        my::vector<int> b = { 1, 2, 3 };
        my::vector<int> c = { 4, 5, 6 };

        a.insert(a.cend(), c.cbegin(), c.cend());
        b.push_back(c[0]);
        b.push_back(c[1]);
        b.push_back(c[2]);

        CHECK(a == b);
        CHECK(a == my::vector { 1, 2, 3, 4, 5, 6 });
    }

    SUBCASE("Insertion moves elements after insert position forward") {
        my::vector<int> a = { 1, 2, 3, 4 };
        my::vector<int> b = { 123, 123, 123 };

        a.insert(a.cbegin() + 2, b.cbegin(), b.cend());

        CHECK(a == my::vector { 1, 2, 123, 123, 123, 3, 4 });
    }
}

TEST_CASE("Inserting initializer list") {
    SUBCASE("Inserting empty initializer list does nothing") {
        my::vector<int> a = { 1, 2, 3 };
        
        a.insert(a.cbegin(), {});
        a.insert(a.cend(), {});
        a.insert(a.cbegin() + 1, {});

        CHECK(a == my::vector { 1, 2, 3 });
    }

    SUBCASE("Inserting before begin and end has the same effect for empty vector") {
        my::vector<int> a;
        my::vector<int> b;

        a.insert(a.cbegin(), { 1, 2, 3 });
        b.insert(b.cend(), { 1, 2, 3 });

        CHECK(a == b);
        CHECK(b == my::vector<int>{ 1, 2, 3 });
    }

    SUBCASE("Inserting before the end has is the same as push back") {
        my::vector<int> a = { 1, 2, 3 };
        my::vector<int> b = { 1, 2, 3 };

        a.insert(a.cend(), { 4, 5, 6 });
        b.push_back(4);
        b.push_back(5);
        b.push_back(6);

        CHECK(a == b);
        CHECK(a == my::vector { 1, 2, 3, 4, 5, 6 });
    }

    SUBCASE("Insertion moves elements after insert position forward") {
        my::vector<int> a = { 1, 2, 3, 4 };

        a.insert(a.cbegin() + 2, { 123, 123, 123 });

        CHECK(a == my::vector { 1, 2, 123, 123, 123, 3, 4 });
    }
}

TEST_CASE("Erasing single element") {
    SUBCASE("Erasing does nothing if vector is empty") {
        my::vector<int> vec = { };
        
        vec.erase(vec.cbegin());
        vec.erase(vec.cend());

        CHECK(vec.empty());
    }

    SUBCASE("Erasing every element leaves empty vector") {
        my::vector<int> vec = { 1, 2, 3 };

        vec.erase(vec.cbegin());
        vec.erase(vec.cbegin());
        vec.erase(vec.cbegin());

        CHECK(vec.empty());
    }

    SUBCASE("Erasing element reduces size") {
        my::vector<int> vec = { 1, 2, 3 };

        const auto size_before = vec.size();
        vec.erase(vec.cbegin());
        const auto size_after = vec.size();

        CHECK(size_after == size_before - 1);
    }
}

TEST_CASE("Erase iterator range") {
    SUBCASE("Erasing empty range does nothing") {
        my::vector<int> vec = { 1, 2, 3 };

        vec.erase(vec.cbegin(), vec.cbegin());
        vec.erase(vec.cend(), vec.cend());

        CHECK(vec == my::vector<int> { 1, 2, 3 });
    }

    SUBCASE("Erasing from empty vector does nothing") {
        my::vector<int> vec = { };

        vec.erase(vec.cbegin(), vec.cbegin());
        vec.erase(vec.cend(), vec.cend());

        CHECK(vec == my::vector<int> { });
    }

    SUBCASE("Erasing all elements results in empty vector") {
        my::vector<int> vec = { 1, 2, 3 };

        vec.erase(vec.cbegin(), vec.cend());

        CHECK(vec == my::vector<int> { });
    }

    SUBCASE("Erasing range of size 1 is the same as erasing single element") {
        my::vector<int> a = { 1, 2, 3 };
        my::vector<int> b = { 1, 2, 3 };

        a.erase(a.cbegin() + 1, a.cbegin() + 2);
        b.erase(b.cbegin() + 1);

        CHECK(a == b);
    }

    SUBCASE("Erasing range actually removes elements") {
        my::vector<int> vec = { 1, 2, 3, 4, 5, 6 };

        vec.erase(vec.cbegin() + 2, vec.cbegin() + 4);

        CHECK(vec == my::vector<int> { 1, 2, 5, 6 });
    }

    SUBCASE("Erasing range reduces size") {
        my::vector<int> vec = { 1, 2, 3, 4, 5, 6 };

        const std::size_t range_size = 3;
        auto range_begin = vec.cbegin() + 2;
        auto range_end = range_begin + range_size;

        const auto size_before = vec.size();
        vec.erase(range_begin, range_end);
        const auto size_after = vec.size();

        CHECK(size_after == size_before - range_size);
    }
}

// Ordinary iterator
TEST_CASE("Forward iteration") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);

    REQUIRE(std::forward_iterator<decltype(vec.begin())>);
    REQUIRE(std::sentinel_for<decltype(vec.end()), decltype(vec.begin())>);

    std::size_t i = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it, ++i) {
        CHECK(*it == vec[i]);
    }
}

TEST_CASE("Bidirectional iteration") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);
    
    REQUIRE(std::bidirectional_iterator<decltype(vec.begin())>);
    REQUIRE(std::bidirectional_iterator<decltype(vec.end())>);

    // Iterate forward
    std::size_t i = 0;
    auto it = vec.begin();
    for (; it != vec.end(); ++it, ++i) {
        CHECK(*it == vec[i]);
    }

    // Iterate backwards
    while (true) {
        CHECK(vec[i] == *it);
        if (it == vec.begin()) {
            break;
        }
        --it;
        --i;
    }
}

TEST_CASE("Random access") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);
    
    REQUIRE(std::random_access_iterator <decltype(vec.begin())>);
    REQUIRE(std::random_access_iterator <decltype(vec.end())>);

    for (std::size_t i = 0; i < vec.size(); i += 1) {
        CHECK(vec.begin()[i] == vec[i]);
    }
}

TEST_CASE("Iterators order") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.begin() == vec.end());
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.begin() < vec.end());
    }
}

TEST_CASE("Iterators distance") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.end() - vec.begin() == 0);
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.end() - vec.begin() == vec.size());
    }
}

TEST_CASE("Iterators advance") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.begin() + 0 == vec.end());
        CHECK(0 + vec.begin() == vec.end());
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.begin() + vec.size() == vec.end());
        CHECK(vec.size() + vec.begin() == vec.end());
    }

    SUBCASE("Negative advance") {
        my::vector<int> vec(10);
        CHECK(vec.end() - vec.size() == vec.begin());
    }
}

TEST_CASE("Range based for loop") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);

    std::size_t i = 0;
    for (const auto num : vec) {
        CHECK(num == vec[i]);
        i += 1;
    }
}

// Const iterator
// Ordinary iterator
TEST_CASE("Const iterator forward iteration") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);

    REQUIRE(std::forward_iterator<decltype(vec.cbegin())>);
    REQUIRE(std::sentinel_for<decltype(vec.cend()), decltype(vec.cbegin())>);

    std::size_t i = 0;
    for (auto it = vec.cbegin(); it != vec.cend(); ++it, ++i) {
        CHECK(*it == vec[i]);
    }
}

TEST_CASE("Const iterator bidirectional iteration") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);
    
    REQUIRE(std::bidirectional_iterator<decltype(vec.cbegin())>);
    REQUIRE(std::bidirectional_iterator<decltype(vec.cend())>);

    // Iterate forward
    std::size_t i = 0;
    auto it = vec.cbegin();
    for (; it != vec.cend(); ++it, ++i) {
        CHECK(*it == vec[i]);
    }

    // Iterate backwards
    while (true) {
        CHECK(vec[i] == *it);
        if (it == vec.cbegin()) {
            break;
        }
        --it;
        --i;
    }
}

TEST_CASE("Const iterator random access") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);
    
    REQUIRE(std::random_access_iterator <decltype(vec.cbegin())>);
    REQUIRE(std::random_access_iterator <decltype(vec.cend())>);

    for (std::size_t i = 0; i < vec.size(); i += 1) {
        CHECK(vec.cbegin()[i] == vec[i]);
    }
}

TEST_CASE("Const iterator iterators order") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.cbegin() == vec.cend());
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.cbegin() < vec.cend());
    }
}

TEST_CASE("Const iterator iterators distance") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.cend() - vec.cbegin() == 0);
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.cend() - vec.cbegin() == vec.size());
    }
}

TEST_CASE("Const iterator iterators advance") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.cbegin() + 0 == vec.cend());
        CHECK(0 + vec.cbegin() == vec.cend());
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.cbegin() + vec.size() == vec.cend());
        CHECK(vec.size() + vec.cbegin() == vec.cend());
    }

    SUBCASE("Negative advance") {
        my::vector<int> vec(10);
        CHECK(vec.cend() - vec.size() == vec.cbegin());
    }
}

TEST_CASE("Const iterator range based for loop") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);

    std::size_t i = 0;
    for (const auto& num : vec) {
        CHECK(num == vec[i]);
        i += 1;
    }
}

// Reverse iterator
TEST_CASE("Reverse forward iteration") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);

    REQUIRE(std::forward_iterator<decltype(vec.rbegin())>);
    REQUIRE(std::sentinel_for<decltype(vec.rend()), decltype(vec.rbegin())>);

    std::size_t i = vec.size() - 1;
    for (auto it = vec.rbegin(); it != vec.rend(); ++it, --i) {
        CHECK(*it == vec[i]);
    }
}

TEST_CASE("Reverse bidirectional iteration") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);
    
    REQUIRE(std::bidirectional_iterator<decltype(vec.rbegin())>);
    REQUIRE(std::bidirectional_iterator<decltype(vec.rend())>);

    // Iterate forward
    std::size_t i = vec.size() - 1;
    auto it = vec.rbegin();
    for (; it != vec.rend(); ++it, --i) {
        CHECK(*it == vec[i]);
    }

    // Iterate backwards
    while (true) {
        CHECK(vec[i] == *it);
        if (it == vec.rbegin()) {
            break;
        }
        --it;
        ++i;
    }
}

TEST_CASE("Reverse iterator random access") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);
    
    REQUIRE(std::random_access_iterator <decltype(vec.rbegin())>);
    REQUIRE(std::random_access_iterator <decltype(vec.rend())>);

    for (std::size_t i = 0; i < vec.size(); i += 1) {
        CHECK(vec.rbegin()[vec.size() - 1 - i] == vec[i]);
    }
}

TEST_CASE("Reverse range based for loop") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);

    std::size_t i = vec.size() - 1;
    for (const auto num : vec | std::views::reverse) {
        CHECK(num == vec[i]);
        i -= 1;
    }
}

TEST_CASE("Reverse iterators order") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.rbegin() == vec.rend());
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.rbegin() < vec.rend());
    }
}

TEST_CASE("Reverse iterators distance") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.rend() - vec.rbegin() == 0);
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.rend() - vec.rbegin() == vec.size());
    }
}

TEST_CASE("Reverse iterators advance") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.rbegin() + 0 == vec.rend());
        CHECK(0 + vec.rbegin() == vec.rend());
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.rbegin() + vec.size() == vec.rend());
        CHECK(vec.size() + vec.rbegin() == vec.rend());
    }

    SUBCASE("Negative advance") {
        my::vector<int> vec(10);
        CHECK(vec.rend() - vec.size() == vec.rbegin());
    }
}

// Const reverse iterator
TEST_CASE("Const reverse iterator forward iteration") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);

    REQUIRE(std::forward_iterator<decltype(vec.crbegin())>);
    REQUIRE(std::sentinel_for<decltype(vec.crend()), decltype(vec.crbegin())>);

    std::size_t i = vec.size() - 1;
    for (auto it = vec.crbegin(); it != vec.crend(); ++it, --i) {
        CHECK(*it == vec[i]);
    }
}

TEST_CASE("Const reverse iterator bidirectional iteration") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);
    
    REQUIRE(std::bidirectional_iterator<decltype(vec.crbegin())>);
    REQUIRE(std::bidirectional_iterator<decltype(vec.crend())>);

    // Iterate forward
    std::size_t i = vec.size() - 1;
    auto it = vec.crbegin();
    for (; it != vec.crend(); ++it, --i) {
        CHECK(*it == vec[i]);
    }

    // Iterate backwards
    while (true) {
        CHECK(vec[i] == *it);
        if (it == vec.crbegin()) {
            break;
        }
        --it;
        ++i;
    }
}

TEST_CASE("Const reverse iterator random access") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);
    
    REQUIRE(std::random_access_iterator <decltype(vec.crbegin())>);
    REQUIRE(std::random_access_iterator <decltype(vec.crend())>);

    for (std::size_t i = 0; i < vec.size(); i += 1) {
        CHECK(vec.crbegin()[vec.size() - 1 - i] == vec[i]);
    }
}

TEST_CASE("Const reverse iterator range based for loop") {
    my::vector<int> vec;
    vec.push_back(111);
    vec.push_back(222);
    vec.push_back(333);

    std::size_t i = vec.size() - 1;
    for (const auto& num : vec | std::views::reverse) {
        CHECK(num == vec[i]);
        i -= 1;
    }
}

TEST_CASE("Const reverse iterators order") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.crbegin() == vec.crend());
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.crbegin() < vec.crend());
    }
}

TEST_CASE("Const reverse iterators distance") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.crend() - vec.crbegin() == 0);
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.crend() - vec.crbegin() == vec.size());
    }
}

TEST_CASE("Const reverse iterators advance") {
    SUBCASE("Empty vector") {
        my::vector<int> vec;
        CHECK(vec.crbegin() + 0 == vec.crend());
        CHECK(0 + vec.crbegin() == vec.crend());
    }

    SUBCASE("Non-empty vector") {
        my::vector<int> vec(10);
        CHECK(vec.crbegin() + vec.size() == vec.crend());
        CHECK(vec.size() + vec.crbegin() == vec.crend());
    }

    SUBCASE("Negative advance") {
        my::vector<int> vec(10);
        CHECK(vec.crend() - vec.size() == vec.crbegin());
    }
}

TEST_CASE("Contiguous iterator") {
    my::vector<int> vec(10);
    
    REQUIRE(std::contiguous_iterator <decltype(vec.begin())>);
    REQUIRE(std::contiguous_iterator <decltype(vec.end())>);
}

TEST_CASE("Constant contiguous iterator") {
    my::vector<int> vec(10);
    
    REQUIRE(std::contiguous_iterator <decltype(vec.cbegin())>);
    REQUIRE(std::contiguous_iterator <decltype(vec.cend())>);
}

TEST_CASE("Custom allocators") {
    // Basic smoke test
    my::vector<int, std::pmr::polymorphic_allocator<int>> vec = { 1, 2, 3 };
    CHECK(vec.size() == 3);
    CHECK(vec.back() == 3);

    vec.pop_back();
    CHECK(vec.size() == 2);
    CHECK(vec.back() == 2);
}