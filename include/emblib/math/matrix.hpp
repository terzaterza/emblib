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

#else



#endif

/** @todo Can make matrix class which extends matrix_base, and matrix_base can be glm::matrix, eigen::matrix, ... */

}