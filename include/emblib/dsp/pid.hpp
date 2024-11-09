#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::dsp {

template <typename scalar_type, size_t DIM = 1>
class pid {

public:
    using vector_t = math::vector<scalar_type, DIM>;
    using vector_bool_t = math::vector<bool, DIM>;

    pid(scalar_type kp,
        scalar_type ki,
        scalar_type kd
    ) noexcept :
        m_kp(kp),
        m_ki(ki),
        m_kd(kd),
        m_clamp(false)
    {}

    pid(
        scalar_type kp,
        scalar_type ki,
        scalar_type kd,
        vector_t clamp_low,
        vector_t clamp_high
    ) noexcept :
        m_kp(kp),
        m_ki(ki),
        m_kd(kd),
        m_clamp(true),
        m_clamp_low(clamp_low),
        m_clamp_high(clamp_high)
    {}

    /**
     * Update PID state with the next input value
    */
    vector_t process(const vector_t& input, const scalar_type& dt) noexcept
    {
        m_output = m_kp * input + (m_ki * dt) * m_integral + (m_kd / dt) * (input - m_prev_input);

        if (m_clamp) {
            /* Only contributing to the integral where not saturated*/
            m_integral += !m_saturated * input;

            /* Check whether there is saturation */
            vector_bool_t sat_high = m_output > m_clamp_high;
            vector_bool_t sat_low = m_output < m_clamp_low;
            m_saturated = sat_high || sat_low;
            
            /* Vector (arithmetic) implementation of clamping the output */
            m_output = !sat_high * m_output + sat_high * m_clamp_high;
            m_output = !sat_low * m_output + sat_low * m_clamp_low;
        } else {
            m_integral += input;
        }
        
        m_prev_input = input;
        return m_output;
    }

    vector_t get_output() const noexcept
    {
        return m_output;
    }

private:
    const scalar_type m_kp;
    const scalar_type m_ki;
    const scalar_type m_kd;

    bool m_clamp;
    vector_t m_clamp_low;
    vector_t m_clamp_high;
    vector_bool_t m_saturated;

    vector_t m_output {0};
    vector_t m_integral {0};
    vector_t m_prev_input {0};

};

}