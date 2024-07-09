#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::drivers::sensor {

template <typename raw_scalar_t = int16_t>
class gyroscope {

public:
    using raw_data = math::vector<raw_scalar_t, 3>;

    /**
     * Sensitivity is the conversion rate from raw data to radians per second [s^-1]
    */
    explicit gyroscope(float default_sensitivity) noexcept :
        sensitivity(default_sensitivity) {}

    /**
     * Read raw gyroscope data
    */
    virtual status read_raw(raw_data& vec) noexcept = 0;

    /**
     * Read and convert data with current sensitivity
     */
    status read(math::vec3f& vec) noexcept
    {
        raw_data raw;

        if (read_raw(raw) != status::OK) {
            return status::ERROR;
        }

        return math::vec3f(raw) * sensitivity;
    }

    /**
     * Public getter for the sensitivity determined by currently set range
    */
    float get_sensitivity() const noexcept
    {
        return sensitivity;
    }

protected:
    /**
     * Derived classes should update this values from set_range method
     * which takes in a parameter from an enum of possible values
     */
    float sensitivity;

};

}