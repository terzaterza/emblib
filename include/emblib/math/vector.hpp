#pragma once

#include "emblib/emblib.hpp"
#include "./matrix.hpp"

namespace emblib::math {

/**
 * Vector is defined as a 1-column matrix
 */
template <typename scalar_type, size_t DIM>
using vector = matrix<scalar_type, DIM, 1>;


template <size_t DIM>
using vectorf = vector<float, DIM>;


using vector3f = vectorf<3>;

}