#pragma once

#include "emblib/emblib.hpp"
#include "./matrix.hpp"

namespace emblib::math {

/**
 * Vector is a 1-column matrix
 */
template <
    typename scalar_type,
    size_t DIM,
    typename base_type = matrix_native_t<scalar_type, DIM, 1>
>
class vector : public matrix<scalar_type, DIM, 1, base_type> {

public:
    template <typename other_base>
    using vector_same_t = vector<scalar_type, DIM, other_base>;

    vector(const std::initializer_list<scalar_type>& elements) :
        matrix<scalar_type, DIM, 1, base_type>({elements}) {}

    scalar_type operator()(size_t idx) const
    {
        return matrix<scalar_type, DIM, 1, base_type>::operator()(idx, 0);
    }

    scalar_type& operator()(size_t idx)
    {
        return matrix<scalar_type, DIM, 1, base_type>::operator()(idx, 0);
    }

    /**
     * Dot product
     */
    template <typename rhs_base>
    scalar_type dot(const vector_same_t<rhs_base>& rhs) const noexcept
    {
        const auto res = this->transpose().matmul(rhs);
        return res(0, 0);
    }

    /**
     * Cross product
     */
    template <typename rhs_base>
    vector cross(const vector_same_t<rhs_base>& rhs) const noexcept
    {
        static_assert(DIM == 3);
        auto lhs = *this;
        return vector {
            lhs(1) * rhs(2) - lhs(2) * rhs(1),
            lhs(2) * rhs(0) - lhs(0) * rhs(2),
            lhs(0) * rhs(1) - lhs(1) * rhs(0)
        };
    }
};


template <size_t DIM>
using vectorf = vector<float, DIM>;


using vector3f = vectorf<3>;

}