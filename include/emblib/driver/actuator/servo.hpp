#pragma once

#include "emblib/emblib.hpp"

namespace emblib::driver {

class servo {

public:
    explicit servo() = default;
    virtual ~servo() = default;

    // TODO: Remove copy and move constrcturos

    /**
     * Set the desired position (angle) for the servo
     * @note Should be between min and max angles
     */
    virtual bool write_angle() noexcept = 0;

    /**
     * Read the current servo position (angle)
     */
    virtual bool read_angle() noexcept = 0;

    /**
     * Get the minimum angle
     */
    virtual float get_min_angle() const noexcept = 0;

    /**
     * Get the maximum angle
     */
    virtual float get_max_angle() const noexcept = 0;
};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif