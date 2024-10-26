#pragma once

#include "emblib/emblib.hpp"
#include <array>
#include <type_traits>

namespace emblib::math::custom {

/**
 * Matrix data view
 */
template <typename scalar_t, size_t rows, size_t cols>
class matrix_view {

public:
    constexpr size_t get_rows() const noexcept
    {
        return rows;
    }

    constexpr size_t get_cols() const noexcept
    {
        return cols;
    }

    virtual constexpr scalar_t& at(size_t row, size_t col) = 0;
    virtual constexpr const scalar_t& at(size_t row, size_t col) const = 0;

};


/**
 * Matrix internal storage
 */
template <typename scalar_t, size_t rows, size_t cols>
class matrix_internal_data_view : public matrix_view<scalar_t, rows, cols> {

public:
    matrix_internal_data_view() {}
    
    matrix_internal_data_view(const std::array<scalar_t, rows*cols>& data)
    {
        std::copy(data.begin(), data.end(), &m_data[0][0]);
    }

    constexpr scalar_t& at(size_t row, size_t col) override
    {
        assert(row < rows && col < cols);
        return m_data[row][col];
    }

    constexpr const scalar_t& at(size_t row, size_t col) const override
    {
        assert(row < rows && col < cols);
        return m_data[row][col];
    }

    constexpr void fill(const scalar_t& value) noexcept
    {
        std::fill(&m_data[0][0], &m_data[0][0] + sizeof(m_data) / sizeof(m_data[0][0]), value);
    }

private:
    // std::array<std::array<scalar_t, cols>, rows> m_data;
    scalar_t m_data[rows][cols];

};


/**
 * Matrix transpose view
 */
template <typename scalar_t, size_t rows, size_t cols, typename subview_t>
class matrix_transpose_view : public matrix_view<scalar_t, rows, cols> {

public:
    constexpr matrix_transpose_view(subview_t& subview) :
        m_view(subview)
    {
        static_assert(std::is_base_of<matrix_view<scalar_t, cols, rows>, subview_t>::value);
    }

    constexpr scalar_t& at(size_t row, size_t col) override
    {
        assert(row < rows && col < cols);
        return m_view.at(col, row);
    }

    constexpr const scalar_t& at(size_t row, size_t col) const override
    {
        assert(row < rows && col < cols);
        return m_view.at(col, row);
    }

    /** @todo To make a copy of this transposed view, pass this as an arg to matrix constructor */

private:
    subview_t& m_view;
};


/** Add submatrix view, sparse view */

}