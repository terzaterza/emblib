#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/vector.hpp"
#include "emblib/algo/iir.hpp"

namespace emblib::algo {

template <typename scalar_t, bool clamp = false>
class pid {

public:
    enum class saturation {
        NONE, LOW, HIGH
    };

    explicit pid(
        scalar_t P,
        scalar_t I,
        scalar_t D,
        scalar_t dt,
        scalar_t clamp_low = 0,
        scalar_t clamp_high = 0
    ) noexcept : P(P), I(I), D(D), dt(dt), clamp_low(clamp_low), clamp_high(clamp_high)
    {}

    /**
     * Update PID state with the next input value
    */
    scalar_t update(scalar_t input) noexcept
    {
        scalar_t next_integral = integral + input;

        output = P * input + I * dt * next_integral + D * (input - prev_input) / dt;

        if constexpr(clamp) {
            /* Contribute to integral only if not adding to saturation */
            if ((saturation != saturation::HIGH && input >= 0) ||
                (saturation != saturation::LOW && input <= 0)) {
                integral = next_integral;
            }

            /* Check whether there is saturation */
            saturation = saturation::NONE;
            if (output > clamp_high) {
                output = clamp_high;
                saturation = saturation::HIGH;
            }
            if (output < clamp_low) {
                output = clamp_low;
                saturation = saturation::LOW;
            }
        } else {
            integral = next_integral;
        }
        
        prev_input = input;
        return output;
    }

    scalar_t get_output() const noexcept
    {
        return output;
    }

    saturation get_saturation() const noexcept
    {
        return saturation;
    }

private:
    scalar_t P;
    scalar_t I;
    scalar_t D;
    scalar_t dt;

    scalar_t output = 0;
    scalar_t integral = 0;
    scalar_t prev_input = 0;

    scalar_t clamp_low = 0;
    scalar_t clamp_high = 0;
    saturation saturation = saturation::NONE;

};

}