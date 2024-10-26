#pragma once

#include "emblib/emblib.hpp"
#include "emblib/algo/iir.hpp"

namespace emblib::algo {

template <typename scalar_t, typename coeff_t = scalar_t>
class pid {

public:
    enum class saturation {
        NONE, LOW, HIGH
    };

    pid(coeff_t P, coeff_t I, coeff_t D) noexcept :
        P(P), I(I), D(D), clamp(false), clamp_low{0}, clamp_high{0}
    {}

    pid(coeff_t P, coeff_t I, coeff_t D, scalar_t clamp_low, scalar_t clamp_high) noexcept :
        P(P), I(I), D(D), clamp(true), clamp_low(clamp_low), clamp_high(clamp_high)
    {}

    /**
     * Update PID state with the next input value
    */
    scalar_t update(const scalar_t& input, coeff_t dt) noexcept
    {
        scalar_t next_integral = integral + input;

        output = P * input + I * dt * next_integral + D * (input - prev_input) / dt;

        if (clamp) {
            // /* Contribute to integral only if not adding to saturation */
            // if ((input >= zero_scalar && saturation != saturation::HIGH) ||
            //     (input <= zero_scalar && saturation != saturation::LOW)) {
            //     integral = next_integral;
            // }

            // /* Check whether there is saturation */
            // saturation = saturation::NONE;
            // if (output > clamp_high) {
            //     output = clamp_high;
            //     saturation = saturation::HIGH;
            // }
            // if (output < clamp_low) {
            //     output = clamp_low;
            //     saturation = saturation::LOW;
            // }
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
    coeff_t P;
    coeff_t I;
    coeff_t D;

    scalar_t output{0};
    scalar_t integral{0};
    scalar_t prev_input{0};

    const bool clamp;
    scalar_t clamp_low;
    scalar_t clamp_high;
    saturation saturation = saturation::NONE;
    static constexpr scalar_t zero_scalar{0};

};

}