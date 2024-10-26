#pragma once

#include "emblib/emblib.hpp"
#include <cmath>

namespace emblib::drivers {

class motor_driver {

public:
    /**
     * Conversion rate from radians per second to rotations per minute
     */
    static constexpr float RPM_TO_RADPS = M_PIf / 30.f;

public:
    /**
     * Set rotation speed of the motor
     * @note Speed units are RPM
     */
    virtual bool set_speed(float rpm) noexcept = 0;

    /**
     * Read the current speed of the motor
     * @note Speed units are RPM
     */
    virtual bool get_speed(float& rpm) noexcept = 0;
};

}