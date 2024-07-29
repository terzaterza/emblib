#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::drivers {

class accelerometer {

public:
    /**
     * Conversion rate from standard gravity to meters per second^2
     */
    static constexpr float G_TO_MPS2 = 9.80665f;

    /**
     * Default constructor and destructor
    */
    explicit accelerometer() = default;
    virtual ~accelerometer() = default;

    /**
     * Check if accelerometer identifies correctly
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
     * Noise spectral density in units [g/sqrt(Hz)]
     */
    virtual float get_noise_density() const noexcept = 0;

    /**
     * Read data into the `vec` argument in standard gravity [g] units
     * @note Should not modify `vec` if read fails
     */
    virtual bool read(math::vec3f& vec) noexcept = 0;

};

}