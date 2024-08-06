#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/math/vector.hpp"

#include <cmath>

namespace emblib::drivers {

class gyroscope {

public:
    /**
     * Conversion rate from degrees to radians
     */
    static constexpr float DEG_TO_RAD = M_PIf / 180.f;

    /**
     * Default constructor and destructor
    */
    explicit gyroscope() = default;
    virtual ~gyroscope() = default;

    /**
     * Check if gyroscope identifies correctly
     * @note For external devices this could be a read of the ID (WHO_AM_I)
     * register of the device and comparing it to the expected value
     * @return `true` if register read successfully and data matches
     */
    virtual bool validate_device() noexcept = 0;

    /**
     * Check if there is new data available since the last read
     */
    virtual bool data_available() noexcept = 0;

    /**
     * Noise spectral density in units [dps/sqrt(Hz)]
     */
    virtual float get_noise_density() const noexcept = 0;

    /**
     * Read data into the `vec` argument in degrees per second [dps] units
     */
    virtual bool read(math::vec3f& vec) noexcept = 0;

    /**
     * Calculate bias by finding the average of `n` readings
     * @note Requires passing a delay function to make a pause between readings
     * @returns true if all readings were successful and bias calculated
     */
    bool calculate_bias(math::vec3f& bias, size_t n, std::function<void()> read_delay) noexcept;

};

/**
 * Inline implementation
 * @todo Move to cpp file
 */
inline bool gyroscope::calculate_bias(math::vec3f& bias, size_t n, std::function<void()> read_delay) noexcept
{
    math::vec3f data;
    bias.fill(0);

    for (size_t i = 0; i < n; i++) {
        if (!read(data)) {
            return false;
        }

        bias += data;
        read_delay();
    }

    bias /= static_cast<float>(n);
    return true;
}

}