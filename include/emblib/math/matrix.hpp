#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_MATH_USE_GLM
    #include "glm/matrix.hpp"
#elif EMBLIB_MATH_USE_EIGEN
    #define EIGEN_NO_MALLOC
    #include "Eigen/Dense"
#elif EMBLIB_MATH_USE_XTENSOR
    #define XTENSOR_ENABLE_ASSERT
    #define XTENSOR_CHECK_DIMENSIONS
    #include "xtensor/xfixed.hpp"
    #include "xtensor-blas/xlinalg.hpp"
#else
    #include <array>
#endif

namespace emblib::math {

#if EMBLIB_MATH_USE_GLM
    template <typename scalar_t, size_t rows, size_t cols>
    using matrix = glm::mat<cols, rows, scalar_t, glm::defaultp>;
#elif EMBLIB_MATH_USE_EIGEN
    template <typename scalar_type, size_t ROWS, size_t COLS = ROWS>
    using matrix_native_t = Eigen::Matrix<scalar_type, ROWS, COLS>;
#else
    #error "Matrix implementation not defined"
#endif


/**
 * Matrix
 */
template <
    typename scalar_type,
    size_t ROWS,
    size_t COLS = ROWS,
    typename base_type = matrix_native_t<scalar_type, ROWS, COLS>
>
class matrix {

public:
    template <typename other_base>
    using matrix_same_t = matrix<scalar_type, ROWS, COLS, other_base>;
    template <typename other_scalar, typename other_base>
    using matrix_shaped_t = matrix<other_scalar, ROWS, COLS, other_base>;
    template <typename other_base>
    using matrix_bool_t = matrix<bool, ROWS, COLS, other_base>;

    /**
     * Base constructor for creating this wrapper from implementation types
     */
    explicit matrix(base_type base) noexcept : m_base(base) {}

    /**
     * Base constructor for creating this out of another base type
     */
    template <typename other_base>
    matrix(const matrix_same_t<other_base>& other) noexcept : m_base(other.get_base()) {}

    /**
     * Initialize all the elements of the matrix to `scalar`
     */
    matrix(scalar_type scalar) noexcept;

    /**
     * Initialize the matrix with the elements
     * @todo Change to fixed size arrays
     */
    matrix(const std::initializer_list<std::initializer_list<scalar_type>>& elements) noexcept;

    /**
     * Get reference to the underlying matrix expression
     */
    const base_type& get_base() const noexcept
    {
        return m_base;
    }

    /**
     * Cast to a different scalar type
     */
    template <typename cast_type>
    auto cast() const noexcept
    {
        auto casted_base = cast_base<cast_type>();
        return matrix<cast_type, ROWS, COLS, decltype(casted_base)>(casted_base);
    }

    /**
     * Get element
     */
    scalar_type operator()(size_t row, size_t col) const
    {
        return m_base(row, col);
    }

    /**
     * Transpose
     */
    auto transpose() const noexcept;

    /**
     * Element-wise addition
     */
    template <typename rhs_base>
    auto operator+(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise subtraction
     */
    template <typename rhs_base>
    auto operator-(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise multiplication
     */
    template <typename rhs_base>
    auto operator*(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise division
     */
    template <typename rhs_base>
    auto operator/(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise less than
     */
    template <typename rhs_base>
    auto operator<(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise less than or equal to
     */
    template <typename rhs_base>
    auto operator<=(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise equal
     */
    template <typename rhs_base>
    auto operator==(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise logical and
     */
    template <typename rhs_base>
    auto operator&&(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise logical or
     */
    template <typename rhs_base>
    auto operator||(const matrix_same_t<rhs_base>& rhs) const noexcept;

    /**
     * Element-wise logical not
     */
    auto operator!() const noexcept;

    /**
     * Are all elements non-null
     */
    bool all() const noexcept;

    /**
     * Are any elements non-null
     */
    bool any() const noexcept;

    /**
     * Matrix multiplication
     */
    template <size_t COLS_RHS, typename rhs_base>
    auto matmul(const matrix<scalar_type, COLS, COLS_RHS, rhs_base>& rhs) const noexcept;

    /**
     * Equivalent to multiplying this matrix from the left by the inverse of the divisor
     */
    template <typename divisor_base>
    auto matdivl(const matrix<scalar_type, ROWS, ROWS, divisor_base>& divisor) const noexcept;

    /**
     * Equivalent to multiplying this matrix from the right by the inverse of the divisor
     */
    template <typename divisor_base>
    auto matdivr(const matrix<scalar_type, COLS, COLS, divisor_base>& divisor) const noexcept
    {
        return matrix<scalar_type, ROWS, COLS>(transpose().matdivl(divisor.transpose()).transpose());
    }

private:
    template <typename cast_type>
    auto cast_base() const noexcept;

private:
    base_type m_base;
};

/** @todo Add external operators for scalar + matrix, ... */

/**
 * Convinience typedef for floating point matrices
 */
template <size_t ROWS, size_t COLS = ROWS>
using matrixf = matrix<float, ROWS, COLS>;


#if EMBLIB_MATH_USE_GLM
#elif EMBLIB_MATH_USE_EIGEN
    #include "./eigen/matrix_inline.hpp"
#else
#endif

}