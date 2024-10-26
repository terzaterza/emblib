#include "emblib/math/custom/matrix_view.hpp"
#include "catch2/catch_test_macros.hpp"

using namespace emblib::math::custom;

TEST_CASE("Matrix transpose test", "[math][custom][matrix_view]")
{
    constexpr size_t rows = 3;
    constexpr size_t cols = 4;

    matrix_internal_data_view<int, rows, cols> data;
    matrix_transpose_view<int, cols, rows, matrix_internal_data_view<int, rows, cols>> tv(data);
    data.fill(0);
    data.at(0, 3) = 2;

    REQUIRE(data.at(0, 3) == 2);
}