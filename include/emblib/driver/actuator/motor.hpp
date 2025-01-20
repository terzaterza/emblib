#pragma once

#include "emblib/emblib.hpp"
#include <cmath>

namespace emblib {

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
    virtual float get_time_consant() const noexcept = 0;

};

}