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

    REQUIRE(b.matdivl(a).get_base().isApprox(left_div_exp.get_base()));
    REQUIRE(b.matdivr(a).get_base().isApprox(right_div_exp.get_base()));
}

TEST_CASE("Matrix logical", "[math][matrix]")
{
    using emblib::math::matrixf;
    matrixf<2, 2> a = {{1, 2}, {7, 8}};
    matrixf<2, 2> b = {{3, 4}, {5, 6}};
    matrixf<2, 2> c = {{5, 10}, {15, 20}};
    
    REQUIRE(((a < b).cast<float>() * c == matrixf<2, 2>({{5, 10}, {0, 0}})).all());
}


TEST_CASE("Matrix concat vertical", "[math][matrix]")
{
    using emblib::math::matrixf;
    matrixf<1, 2> a {{1, 2}};
    matrixf<2, 2> b {{5, 6}, {7, 8}};

    matrixf<3, 2> result {{1, 2}, {5, 6}, {7, 8}};

    // REQUIRE(a.concat_vertical<2>(b) == result);
}

TEST_CASE("Matrix concat horizontal", "[math][matrix]")
{
    using emblib::math::matrixf;
    matrixf<2, 1> a {{1, 2}};
    matrixf<2, 2> b {{5, 6}, {7, 8}};

    matrixf<2, 3> result {{1, 5, 6}, {2, 7, 8}};

    // REQUIRE(a.concat_horizontal<2>(b) == result);
}

TEST_CASE("Matrix submatrix edit", "[math][matrix]")
{
    using emblib::math::matrixf;
    matrixf<3, 4> a {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    
    auto submatrix = a.get_submatrix<1, 2, 2, 2>();
    submatrix(0, 0) = 20;
    submatrix(1, 1) = 30;

    matrixf<3, 4> expected {{1, 2, 3, 4}, {5, 6, 20, 8}, {9, 10, 11, 30}};
    REQUIRE((a == expected).all());
}

TEST_CASE("Matrix submatrix set", "[math][matrix]")
{
    using emblib::math::matrixf;
    matrixf<3, 4> a {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    
    a.set_submatrix(1, 1, matrixf<2>::diagonal(3));
    
    matrixf<3, 4> expected {{1, 2, 3, 4}, {5, 3, 0, 8}, {9, 0, 3, 12}};
    REQUIRE((a == expected).all());
}