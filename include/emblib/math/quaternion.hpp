#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_MATH_USE_GLM
    #include "glm/matrix.hpp"
#elif EMBLIB_MATH_USE_EIGEN
    #define EIGEN_QUATERNION_PLUGIN "emblib/math/eigen/quaternion_plugin.hpp"
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

template <typename scalar_type>
using quaternion = Eigen::Quaternion<scalar_type>;

#else

#error "No quaternion implementation"

#endif


using quaternionf = quaternion<float>;

}