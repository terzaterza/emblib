#pragma once

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

template <typename scalar_t, size_t dim>
using vector = glm::vec<dim, scalar_t, glm::defaultp>;

using glm::all;

#elif EMBLIB_MATH_USE_EIGEN

template <typename scalar_t, size_t dim>
using vector = Eigen::Vector<scalar_t, dim>;

#else



#endif

/** @todo Can make vector class which extends vector_base, and vector_base can be glm::matrix, eigen::matrix, ... */

template <size_t dim>
using vectorf = vector<float, dim>;

using vec2f = vector<float, 2>;
using vec3f = vector<float, 3>;
using vec4f = vector<float, 4>;


template <typename scalar_t, size_t dim_1, size_t dim_2>
static vector<scalar_t, dim_1 + dim_2> vector_stack(const vector<scalar_t, dim_1>& v1, const vector<scalar_t, dim_2>& v2) noexcept
{
    vector<scalar_t, dim_1 + dim_2> result;
    result << v1, v2;
    return result;
}


template <size_t low, size_t high, typename derived>
static auto subvector(const Eigen::MatrixBase<derived>& v)
{
    return v.template segment<high-low>(low);
}

}