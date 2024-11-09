#include "emblib/math/vector.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Vector dot product", "[math][vector]")
{
    using emblib::math::vectorf;
    vectorf<4> a {1, 2, 3, 4};
    vectorf<4> b {7, 6, 5, 4};

    REQUIRE(a.dot(b) == (7 + 12 + 15 + 16));
}

TEST_CASE("Vector cross product", "[math][vector]")
{
    using emblib::math::vectorf;
    vectorf<3> a {1, 2, 3};
    vectorf<3> b {7, 6, 5};

    REQUIRE((a.cross(b) == vectorf<3> {-8, 16, -8}).all());
}