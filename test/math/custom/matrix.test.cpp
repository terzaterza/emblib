#include "emblib/math/custom/matrix.hpp"
#include "catch2/catch_test_macros.hpp"

using namespace emblib::math::custom;

TEST_CASE("Matrix add test", "[math][custom][matrix]")
{
    constexpr size_t rows = 3;
    constexpr size_t cols = 4;

    matrix<int, rows, cols> a, b;
    a.fill(0);
    b.fill(0);
    a.at(2, 3) = 4;
    b.at(2, 3) = 7;

    REQUIRE((a + b).at(2, 3) == 11);
}

TEST_CASE("Matrix multiply test", "[math][custom][matrix]")
{
    constexpr size_t rows = 2;
    constexpr size_t cols = 2;

    matrix<int, rows, cols> a({1, 2, 3, 4});
    matrix<int, rows, cols> b({5, 6, 7, 8});

    REQUIRE(a * b == matrix<int, rows, cols>({19, 22, 43, 50}));
}

TEST_CASE("Matrix transpose test", "[math][custom][matrix]")
{
    constexpr size_t rows = 2;
    constexpr size_t cols = 3;

    matrix<int, rows, cols> a({1, 2, 3, 4, 5, 6});

    REQUIRE(a.transpose() == matrix<int, cols, rows>({1, 4, 2, 5, 3, 6}));
}

TEST_CASE("Matrix compare test", "[math][custom][matrix]")
{
    constexpr size_t rows = 2;
    constexpr size_t cols = 2;

    matrix<int, rows, cols> a({1, 2, 3, 4});
    matrix<int, rows, cols> b({5, 6, 7, 8});

    REQUIRE((b.dot(a > 2 && b <= 7)) == matrix<int, rows, cols>({0, 0, 7, 0}));
}