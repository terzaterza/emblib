#include "emblib/math/matrix.hpp"
#include "catch2/catch_test_macros.hpp"
#include <iostream>

using namespace emblib::math;

TEST_CASE("Matrix multiplication test", "[math][matrix]")
{
    matrix<float, 3, 4> m1;
    matrix<float, 4, 3> m2;

    auto m3 = m1 * m2;

    REQUIRE(m3.cols() == 3);
    REQUIRE(m3.rows() == 3);
}

TEST_CASE("Matrix transpose test", "[math][matrix]")
{
    constexpr size_t rows = 3;
    constexpr size_t cols = 4;

    matrix<float, rows, cols> m;
    auto m_t = m.transpose();

    REQUIRE(m_t.cols() == rows);
    REQUIRE(m_t.rows() == cols);
}

TEST_CASE("Matrix division test", "[math][matrix]")
{
    matrix<float, 2, 2> m, n;
    m << 1, 2, 3, 4;
    n << 5, 6, 7, 8;

    auto left_quot = matrix_divide_left<float, 2, 2>(n, m);
    auto right_quot = matrix_divide_right<float, 2, 2>(m, n);

    matrix<float, 2, 2> left_expect, right_expect;
    left_expect << -3, -4, 4, 5;
    right_expect << 3, -2, 2, -1;

    REQUIRE(left_quot.isApprox(left_expect));
    REQUIRE(right_quot.isApprox(right_expect));
}

TEST_CASE("Matrix comparison test", "[math][matrix]")
{
    matrix<float, 2, 2> m, n;
    m << 1, 2, 3, 4;
    n << 5, 6, 7, 8;

    auto comp = m.array() < 3;
    auto times = comp.cast<float>() * n.array();
    REQUIRE(matrix<float, 2, 2>(times) == matrix<float, 2, 2>{{5, 6}, {0, 0}});
}