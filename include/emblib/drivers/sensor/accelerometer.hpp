#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/math/vector.hpp"

#include <limits>

namespace emblib::drivers::sensor {

template <typename scalar_t = int16_t>
class accelerometer {

using data_vec = math::vector<scalar_t, 3>;

public:
    /**
     * Sensitivity is the conversion rate from raw data to meters per second^2
    */
    explicit accelerometer(float default_sensitivity) noexcept
        : sensitivity(default_sensitivity) {}

    /**
     * Read raw accelerometer data
    */
    virtual status read_raw(data_vec& vec) noexcept = 0;

    /**
     * Set the accelerometer update rate
     * @note `rate` param should be set from an enum dependent on specific accel
    */
    virtual status set_rate(uint32_t rate) noexcept = 0;

    /**
     * Set the accelerometer range in +-abs_range g's
     * @note This function should set internal sensitivity value
     * @note `abs_g` param should be set from an accel specific enum
    */
    virtual status set_range(float abs_g) noexcept = 0;

    /**
     * Public getter for the sensitivity determined by currently set range
    */
    float get_sensitivity() noexcept
    {
        return sensitivity;
    }

private:
    float sensitivity;

};

}