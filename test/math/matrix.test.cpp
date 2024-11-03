#include "emblib/math/matrix.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Matrix division", "[math][matrix]")
{
    using emblib::math::matrixf;
    matrixf<2, 2> a {{1, 2}, {3, 4}};
    matrixf<2, 2> b {{5, 6}, {7, 8}};

    // Result of inv(a) * b
    matrixf<2, 2> left_div_exp {{-3, -4}, {4, 5}};
    // Result of b * inv(a)
    matrixf<2, 2> right_div_exp {{-1, 2}, {-2, 3}};

    REQUIRE(b.divide_left(a).isApprox(left_div_exp));
    REQUIRE(b.divide_right(a).isApprox(right_div_exp));
}


TEST_CASE("Matrix concat vertical", "[math][matrix]")
{
    using emblib::math::matrixf;
    matrixf<1, 2> a {1, 2};
    matrixf<2, 2> b {{5, 6}, {7, 8}};

    matrixf<3, 2> result {{1, 2}, {5, 6}, {7, 8}};

    REQUIRE(a.concat_vertical<2>(b) == result);
}

TEST_CASE("Matrix concat horizontal", "[math][matrix]")
{
    using emblib::math::matrixf;
    matrixf<2, 1> a {1, 2};
    matrixf<2, 2> b {{5, 6}, {7, 8}};

    matrixf<2, 3> result {{1, 5, 6}, {2, 7, 8}};

    REQUIRE(a.concat_horizontal<2>(b) == result);
}