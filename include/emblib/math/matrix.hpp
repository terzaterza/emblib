#pragma once

#include <array>

#include "emblib/emblib.hpp"

#if EMBLIB_MATH_USE_GLM
#include "glm/matrix.hpp"
#elif EMBLIB_MATH_USE_EIGEN
#include "Eigen/Dense"
#else
#include "custom/matrix.hpp"
#endif
/** @todo Add xtensor */


namespace emblib::math {

#if EMBLIB_MATH_USE_GLM

template <typename scalar_t, size_t rows, size_t cols>
using matrix = glm::mat<cols, rows, scalar_t, glm::defaultp>;

#elif EMBLIB_MATH_USE_EIGEN

template <typename scalar_t, size_t rows, size_t cols>
using matrix = Eigen::Matrix<scalar_t, rows, cols>;

#else

template <typename scalar_t, size_t rows, size_t cols>
using matrix = custom::matrix<scalar_t, rows, cols>;

#endif

template <size_t rows, size_t cols = rows>
using matrixf = matrix<float, rows, cols>;

/**
 * Identity matrix creation
 */
template <typename scalar_t, size_t dim>
constexpr matrix<scalar_t, dim, dim> identity() noexcept
{
    matrix<scalar_t, dim, dim> m = matrix<scalar_t, dim, dim>::Zero();
    for (size_t i = 0; i < dim; i++) {
        m(i, i) = 1;
    }
    return m;
}


template <typename scalar_t, size_t dim>
constexpr matrix<scalar_t, dim, dim> diagonal(const std::array<scalar_t, dim>& values) noexcept
{
    matrix<scalar_t, dim, dim> m = matrix<scalar_t, dim, dim>::Zero();
    for (size_t i = 0; i < dim; i++) {
        m(i, i) = values[i];
    }
    return m;
}


template <size_t rlow, size_t rhigh, size_t clow, size_t chigh, typename derived>
static auto submatrix(const Eigen::MatrixBase<derived>& m)
{
    return m.template block<rhigh-rlow, chigh-clow>(rlow, clow);
}


template <typename scalar_t, size_t rows, size_t cols>
constexpr matrix<scalar_t, rows, cols> matrix_divide_left(
    const matrix<scalar_t, cols, cols>& divisor,
    const matrix<scalar_t, rows, cols>& dividend) noexcept
{

#if EMBLIB_MATH_USE_GLM

#elif EMBLIB_MATH_USE_EIGEN
    Eigen::ColPivHouseholderQR<matrix<scalar_t, rows, cols>> dividend_decomp(dividend);
    return dividend_decomp.solve(divisor);
#else

#endif

}

template <typename scalar_t, size_t rows, size_t cols>
constexpr matrix<scalar_t, rows, cols> matrix_divide_right(
    const matrix<scalar_t, rows, cols>& dividend,
    const matrix<scalar_t, cols, cols>& divisor) noexcept
{

#if EMBLIB_MATH_USE_GLM

#elif EMBLIB_MATH_USE_EIGEN
    Eigen::ColPivHouseholderQR<matrix<scalar_t, cols, cols>> divisor_decomp(divisor.transpose());
    return divisor_decomp.solve(dividend.transpose()).transpose();
#else

#endif

}

/** @todo Can make matrix class which extends matrix_base, and matrix_base can be glm::matrix, eigen::matrix, ... */

}