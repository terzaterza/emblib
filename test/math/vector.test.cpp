#include "emblib/math/vector.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Vector stack test", "[math][vector]")
{
    emblib::math::vec3f a(1, 2, 3);
    emblib::math::vec2f b(4, 5);

    auto c = emblib::math::vector_stack<float, 3, 2>(a, b);
    emblib::math::vector<float, 5> result;
    result << 1, 2, 3, 4, 5;

    REQUIRE(c == result);
}