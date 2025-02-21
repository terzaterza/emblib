#pragma once

#include "emblib/emblib.hpp"
#include <cmath>

namespace emblib::driver {

class motor {

public:
    // Conversion rate from RPM to radians per second
    static constexpr float RPM_TO_RAD_PER_SEC = 2 * M_PIf / 60;

public:
    /**
     * Default constructor and destructor
     */
    explicit motor() = default;
    virtual ~motor() = default;

    /**
     * Set the speed of the motor
     * @returns true if successful
     * @todo Change to `set_speed`
     */
    virtual bool write_speed(float rad_per_sec) noexcept = 0;
    
    /**
     * Read the current motor speed
     * @returns true if successful
     */
    virtual bool read_speed(float& rad_per_sec) noexcept = 0;

    /**
     * Assuming a first order approximation of the transfer
     * function in the form:
     * `G(s) = 1 / (1 + s k_t)`
     * return `k_t`
     */
    virtual float get_time_constant() const noexcept = 0;

    /**
     * Return true if the shaft is spinning CCW when
     * looking at the shaft from the front of the motor
     */
    virtual bool get_direction() const noexcept = 0;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif