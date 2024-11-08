#pragma once

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline matrix<scalar_type, ROWS, COLS, base_type>::matrix(scalar_type scalar) noexcept
    : m_base(matrix_native_t<scalar_type, ROWS, COLS>::Constant(scalar))
{
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline matrix<scalar_type, ROWS, COLS, base_type>::matrix(const std::initializer_list<std::initializer_list<scalar_type>>& elements) noexcept
    : m_base(elements)
{
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::transpose() const noexcept
{
    auto res = m_base.transpose();
    return matrix<scalar_type, COLS, ROWS, decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator+(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = m_base + rhs.m_base;
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator-(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = m_base - rhs.m_base;
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator*(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = m_base.cwiseProduct(rhs.m_base);
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::operator/(const matrix_same_t<rhs_base> &rhs) const noexcept
{
    auto res = (m_base.array() / rhs.m_base.array()).matrix();
    return matrix_same_t<decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <size_t COLS_RHS, typename rhs_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::matmul(const matrix<scalar_type, COLS, COLS_RHS, rhs_base> &rhs) const noexcept
{
    auto res = m_base * rhs.m_base;
    return matrix<scalar_type, ROWS, COLS_RHS, decltype(res)>(res);
}

template <typename scalar_type, size_t ROWS, size_t COLS, typename base_type>
template <typename divisor_base>
inline auto matrix<scalar_type, ROWS, COLS, base_type>::matdivl(const matrix<scalar_type, ROWS, ROWS, divisor_base> &divisor) const noexcept
{
    Eigen::Matrix<scalar_type, ROWS, COLS> res = divisor.get_base().colPivHouseholderQr().solve(m_base);
    return matrix_same_t<decltype(res)>(res);
}