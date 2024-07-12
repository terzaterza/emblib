#pragma once

#include <array>

#include "emblib/emblib.hpp"

#if EMBLIB_MATH_USE_GLM
#include "glm/matrix.hpp"
#elif EMBLIB_MATH_USE_EIGEN
#include "Eigen/Dense"
#else
#error "No current math library implementation"
#endif

namespace emblib::math {

#if EMBLIB_MATH_USE_GLM

template <typename scalar_t, size_t rows, size_t cols>
using matrix = glm::mat<cols, rows, scalar_t, glm::defaultp>;

using glm::transpose;
using glm::inverse;

#else

template <typename scalar_t, size_t rows, size_t cols>
using matrix = Eigen::Matrix<scalar_t, rows, cols>;

#endif


/**
 * Identity matrix creation
 */
template <typename scalar_t, size_t dim>
constexpr matrix<scalar_t, dim, dim> identity() noexcept
{
    matrix<scalar_t, dim, dim> m(0);
    for (size_t i = 0; i < dim; i++) {
        m(i, i) = 1;
    }
    return m;
}


template <typename scalar_t, size_t dim>
constexpr matrix<scalar_t, dim, dim> diagonal(const std::array<scalar_t, dim>& values) noexcept
{
    matrix<scalar_t, dim, dim> m(0);
    for (size_t i = 0; i < dim; i++) {
        m(i, i) = values[i];
    }
    return m;
}

/** @todo Can make matrix class which extends matrix_base, and matrix_base can be glm::matrix, eigen::matrix, ... */

}