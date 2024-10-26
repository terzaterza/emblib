#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/custom/matrix_view.hpp"
#include <functional>

namespace emblib::math::custom {

template <typename scalar_t, size_t rows, size_t cols, typename view_t = matrix_internal_data_view<scalar_t, rows, cols>>
class matrix {

public:
    /* Default constructor */
    explicit matrix() noexcept
    {}

    /* Construct with the given elements */
    explicit matrix(const std::array<scalar_t, rows*cols>& elements) :
        m_view(elements)
    {
        static_assert(std::is_base_of<matrix_internal_data_view<scalar_t, rows, cols>, view_t>::value);
    }
    
    /* Construct with a view */
    /** @todo Replace with matrix(matrix<scalar_t, cols, rows>& m) and assert that view_t is transpose view */
    /** @todo Should be private */
    explicit matrix(const view_t& view) : m_view(view)
    {
        static_assert(std::is_base_of<matrix_view<scalar_t, rows, cols>, view_t>::value);
    }

    /**
     * Number of rows
     */
    constexpr size_t get_rows() const noexcept
    {
        return m_view.get_rows();
    }

    /**
     * Number of columns
     */
    constexpr size_t get_cols() const noexcept
    {
        return m_view.get_cols();
    }

    /**
     * Matrix get element
     */
    scalar_t& at(size_t row, size_t col)
    {
        static_assert(std::is_base_of<matrix_view<scalar_t, rows, cols>, view_t>::value);
        return m_view.at(row, col);
    }

    /**
     * Matrix get element
     */
    const scalar_t& at(size_t row, size_t col) const
    {
        static_assert(std::is_base_of<matrix_view<scalar_t, rows, cols>, view_t>::value);
        return m_view.at(row, col);
    }

    /**
     * Matrix fill with a constant value
     */
    void fill(const scalar_t& value)
    {
        static_assert(std::is_base_of<matrix_internal_data_view<scalar_t, rows, cols>, view_t>::value);
        m_view.fill(value);
    }

    /**
     * Returns true if all of the values satisfy the unary condition
     */
    template <typename unary_op_t>
    bool all(const unary_op_t& op = [](const scalar_t& x){return x;}) const noexcept
    {
        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
                if (!this->at(r, c)) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Returns true if any of the values satisfies the unary condition
     */
    template <typename unary_op_t>
    bool any(const unary_op_t& op = [](const scalar_t& x){return x;}) const noexcept
    {
        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
                if (this->at(r, c)) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Matrix transpose
     */
    using matrix_transpose_t = matrix<scalar_t, cols, rows, matrix_transpose_view<scalar_t, cols, rows, view_t>>;
    const matrix_transpose_t transpose()
    {
        return matrix_transpose_t(matrix_transpose_view<scalar_t, cols, rows, view_t>(m_view));
    }

    /* Operator overloads */

    /**
     * Matrix add matrix
     */
    template <typename scalar_other_t, typename view_other_t>
    matrix<scalar_t, rows, cols> operator+(const matrix<scalar_other_t, rows, cols, view_other_t>& other) const noexcept
    {
        return element_wise_op(other, std::plus<scalar_t>());
    }

    /**
     * Matrix add scalar
     */
    matrix<scalar_t, rows, cols> operator+(const scalar_t& scalar) const noexcept
    {
        return scalar_op(scalar, std::plus<scalar_t>());
    }

    /**
     * Matrix sub matrix
     */
    template <typename scalar_other_t, typename view_other_t>
    matrix<scalar_t, rows, cols> operator-(const matrix<scalar_other_t, rows, cols, view_other_t>& other) const noexcept
    {
        return element_wise_op(other, std::minus<scalar_t>());
    }

    /**
     * Matrix sub scalar
     */
    matrix<scalar_t, rows, cols> operator-(const scalar_t& scalar) const noexcept
    {
        return scalar_op(scalar, std::minus<scalar_t>());
    }

    /**
     * Matrix multiply matrix
     */
    template <size_t cols_other, typename scalar_other_t, typename view_other_t>
    matrix<scalar_t, rows, cols_other> operator*(const matrix<scalar_other_t, cols, cols_other, view_other_t>& other) const noexcept;

    /**
     * Matrix multiply scalar
     */
    matrix<scalar_t, rows, cols> operator*(const scalar_t& scalar) const noexcept
    {
        return scalar_op(scalar, std::multiplies<scalar_t>());
    }

    /**
     * Matrix multiply element wise
     */
    template <typename scalar_other_t, typename view_other_t>
    matrix<scalar_t, rows, cols> dot(const matrix<scalar_other_t, rows, cols, view_other_t>& other) const noexcept
    {
        return element_wise_op(other, std::multiplies<scalar_t>());
    }

    /**
     * Matrix less than matrix
     */
    template <typename scalar_other_t, typename view_other_t>
    matrix<bool, rows, cols> operator<(const matrix<scalar_other_t, rows, cols, view_other_t>& other) const noexcept
    {
        return element_wise_op<std::less<scalar_t>, bool>(other, std::less<scalar_t>());
    }

    /**
     * Matrix less than matrix
     */
    matrix<bool, rows, cols> operator<(const scalar_t& scalar) const noexcept
    {
        return scalar_op<std::less<scalar_t>, bool>(scalar, std::less<scalar_t>());
    }

    /**
     * Matrix less than or equal to matrix
     */
    template <typename scalar_other_t, typename view_other_t>
    matrix<bool, rows, cols> operator<=(const matrix<scalar_other_t, rows, cols, view_other_t>& other) const noexcept
    {
        return element_wise_op<std::less_equal<scalar_t>, bool>(other, std::less_equal<scalar_t>());
    }

    /**
     * Matrix less than or equal to matrix
     */
    matrix<bool, rows, cols> operator<=(const scalar_t& scalar) const noexcept
    {
        return scalar_op<std::less_equal<scalar_t>, bool>(scalar, std::less_equal<scalar_t>());
    }

    /**
     * Matrix greater than matrix
     */
    template <typename scalar_other_t, typename view_other_t>
    matrix<bool, rows, cols> operator>(const matrix<scalar_other_t, rows, cols, view_other_t>& other) const noexcept
    {
        return element_wise_op<std::greater<scalar_t>, bool>(other, std::greater<scalar_t>());
    }

    /**
     * Matrix greater than matrix
     */
    matrix<bool, rows, cols> operator>(const scalar_t& scalar) const noexcept
    {
        return scalar_op<std::greater<scalar_t>, bool>(scalar, std::greater<scalar_t>());
    }

    /**
     * Matrix greater than or equal to matrix
     */
    template <typename scalar_other_t, typename view_other_t>
    matrix<bool, rows, cols> operator>=(const matrix<scalar_other_t, rows, cols, view_other_t>& other) const noexcept
    {
        return element_wise_op<std::greater_equal<scalar_t>, bool>(other, std::greater_equal<scalar_t>());
    }

    /**
     * Matrix greater than or equal to matrix
     */
    matrix<bool, rows, cols> operator>=(const scalar_t& scalar) const noexcept
    {
        return scalar_op<std::greater_equal<scalar_t>, bool>(scalar, std::greater_equal<scalar_t>());
    }

    /**
     * Matrix equals
     */
    template <typename view_other_t>
    bool operator==(const matrix<scalar_t, rows, cols, view_other_t>& other) const noexcept;

    /**
     * Matrix and matrix
     */
    template <size_t r, size_t c>
    friend matrix<bool, r, c> operator&&(
        const matrix<bool, r, c>& left,
        const matrix<bool, r, c>& right) noexcept;

    /**
     * Matrix or matrix
     */
    template <size_t r, size_t c>
    friend matrix<bool, r, c> operator||(
        const matrix<bool, r, c>& left,
        const matrix<bool, r, c>& right) noexcept;

protected:
    /**
     * Template for all element-wise operations
     */
    template <typename binary_op_t, typename scalar_other_t, typename view_other_t, typename result_t = scalar_t>
    matrix<result_t, rows, cols> element_wise_op(const matrix<scalar_other_t, rows, cols, view_other_t>& other, const binary_op_t& op) const noexcept
    {
        /* Require here that binary_op_t has `result_t operator(const scalar_t& left, const scalar_t& right) const` */
        matrix<result_t, rows, cols> result;
        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
                result.at(r, c) = op(at(r, c), other.at(r, c));
            }
        }
        return result;
    }

    /**
     * Template for all scalar operations
     */
    template <typename binary_op_t, typename result_t = scalar_t>
    matrix<result_t, rows, cols> scalar_op(const scalar_t& scalar, const binary_op_t& op) const noexcept
    {
        matrix<result_t, rows, cols> result;
        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < cols; c++) {
                result.at(r, c) = op(at(r, c), scalar);
            }
        }
        return result;
    }

private:
    view_t m_view;
    
};



/**
 * Matrix multiplication naive implementation
 */
template <typename scalar_t, size_t rows, size_t cols, typename view_t>
template <size_t cols_other, typename scalar_other_t, typename view_other_t>
inline matrix<scalar_t, rows, cols_other> matrix<scalar_t, rows, cols, view_t>::operator*(const matrix<scalar_other_t, cols, cols_other, view_other_t> &other) const noexcept
{
    matrix<scalar_t, rows, cols_other> result;
    result.fill(0);

    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            const scalar_t s = at(r, c);
            for (size_t i = 0; i < cols_other; i++) {
                result.at(r, i) += s * other.at(c, i);
            }
        }
    }
    return result;
}

/**
 * Matrix (equals) comapre
 */
template <typename scalar_t, size_t rows, size_t cols, typename view_t>
template <typename view_other_t>
inline bool matrix<scalar_t, rows, cols, view_t>::operator==(const matrix<scalar_t, rows, cols, view_other_t> &other) const noexcept
{
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            if (at(r, c) != other.at(r, c)) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Matrix and matrix
 */
template <size_t rows, size_t cols>
matrix<bool, rows, cols> operator&&(
    const matrix<bool, rows, cols>& left,
    const matrix<bool, rows, cols>& right) noexcept
{
    return left.element_wise_op(right, std::logical_and<bool>());
}

/**
 * Matrix or matrix
 */
template <size_t rows, size_t cols>
matrix<bool, rows, cols> operator||(
    const matrix<bool, rows, cols>& left,
    const matrix<bool, rows, cols>& right) noexcept
{
    return left.element_wise_op(right, std::logical_or<bool>());
}

}