#pragma once

#include <cstddef>
#include "Eigen/Dense"

namespace emblib::math::eigen {

template <typename scalar_t, size_t rows, size_t cols>
class matrix : public Eigen::Matrix<scalar_t, rows, cols> {

/** @todo Implement minimal inheritance requirements from Eigen website */

/** @todo Implement operator overloads which accept const Eigen::MatrixBase<dervied>& other and return this class type */

};

}