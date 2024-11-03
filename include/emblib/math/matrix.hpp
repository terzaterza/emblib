#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_MATH_USE_GLM
    #include "glm/matrix.hpp"
#elif EMBLIB_MATH_USE_EIGEN
    #define EIGEN_NO_MALLOC
    #define EIGEN_MATRIXBASE_PLUGIN "emblib/math/eigen/matrix_base_plugin.hpp"
    #include "Eigen/Dense"
#else
    #include <array>
#endif

namespace emblib::math {

#if EMBLIB_MATH_USE_GLM
    template <typename scalar_t, size_t rows, size_t cols>
    using matrix = glm::mat<cols, rows, scalar_t, glm::defaultp>;
#elif EMBLIB_MATH_USE_EIGEN
    /**
     * Matrix implementation via Eigen library
     * @note All extra API methods are provided via plugins defined in this header
     */
    template <typename scalar_type, size_t ROWS, size_t COLS = ROWS>
    using matrix = Eigen::Matrix<scalar_type, ROWS, COLS>;
#else
    #error "Matrix implementation not defined"
#endif

/**
 * Convinience typedef for floating point matrices
 */
template <size_t ROWS, size_t COLS = ROWS>
using matrixf = matrix<float, ROWS, COLS>;

}