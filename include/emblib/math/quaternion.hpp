#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/vector.hpp"

#if EMBLIB_MATH_USE_GLM
#include "glm/matrix.hpp"
#elif EMBLIB_MATH_USE_EIGEN
#include "Eigen/Geometry"
#else
#error "No current math library implementation"
#endif

namespace emblib::math {

#if EMBLIB_MATH_USE_GLM

template <typename scalar_t, size_t dim>
using vector = glm::vec<dim, scalar_t, glm::defaultp>;

using glm::all;

#elif EMBLIB_MATH_USE_EIGEN

template <typename scalar_t>
using quaternion = Eigen::Quaternion<scalar_t>;

#else

#error "No quaternion implementation"

#endif

using quaternionf = quaternion<float>;

template <typename scalar_t>
vector<scalar_t, 3> quaternion_rotate(const quaternion<scalar_t>& q, const vector<scalar_t, 3>& v) noexcept
{
    return q._transformVector(v);
}

}