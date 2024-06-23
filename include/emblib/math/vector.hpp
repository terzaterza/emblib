#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_MATH_USE_GLM
#include "glm/matrix.hpp"
#else
#error "No current math library implementation"
#endif

namespace emblib::math {

#if EMBLIB_MATH_USE_GLM

template <typename scalar_t, size_t dim>
using vector = glm::vec<dim, scalar_t, glm::defaultp>;

using glm::all;

#else



#endif

/** @todo Can make vector class which extends vector_base, and vector_base can be glm::matrix, eigen::matrix, ... */

using vec2f = vector<float, 2>;
using vec3f = vector<float, 3>;

}