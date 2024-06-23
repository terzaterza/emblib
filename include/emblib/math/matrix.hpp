#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_MATH_USE_GLM
#include "glm/matrix.hpp"
#else
#error "No current math library implementation"
#endif

namespace emblib::math {

#if EMBLIB_MATH_USE_GLM

template <typename scalar_t, size_t rows, size_t cols>
using matrix = glm::mat<cols, rows, scalar_t, glm::defaultp>;

using glm::transpose;
using glm::inverse;

template <typename scalar_t, size_t dim>
constexpr matrix<scalar_t, dim, dim> identity() noexcept
{
    matrix<scalar_t, dim, dim> m;
    for (size_t i = 0; i < dim; i++)
        m(i, i) = 1;
    return m;
}

#else



#endif

/** @todo Can make matrix class which extends matrix_base, and matrix_base can be glm::matrix, eigen::matrix, ... */

}