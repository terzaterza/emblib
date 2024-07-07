#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/vector.hpp"

#include "etl/circular_buffer.h"
#include "etl/algorithm.h"

namespace emblib::algo {

/**
 * Direct form II implementation of IIR filter
*/
template <typename scalar_t, size_t num_dim, size_t den_dim>
class iir {

public:
    explicit iir(
        const std::array<scalar_t, num_dim>& num,
        const std::array<scalar_t, den_dim>& den
    ) noexcept : num(num), den(den)
    {}
    
    /**
     * Compute next output and update the delay line
    */
    scalar_t update(const scalar_t input) noexcept
    {
        scalar_t delay_next = input;

        for (size_t i = 1; i < den_dim; i++) {
            delay_next -= den[i] * delay_line[i-1];
        }

        output = num[0] * delay_next;
        for (size_t i = 1; i < num_dim; i++) {
            output += num[i] * delay_line[i-1];
        }

        delay_line.push(delay_next);

        output /= den[0];
        return output;
    }

    /**
     * Get last computed output
    */
    scalar_t get_output() const noexcept
    {
        return output;
    }

    
private:
    std::array<scalar_t, num_dim> num;
    std::array<scalar_t, den_dim> den;
    etl::circular_buffer<scalar_t, etl::max(num_dim, den_dim)-1> delay_line;

    scalar_t output = 0;
};

}