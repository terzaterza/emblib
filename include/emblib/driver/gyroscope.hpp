#pragma once

#include "emblib/emblib.hpp"
#include "three_axis_sensor.hpp"
#include <cmath>

namespace emblib::driver {

class gyroscope : public three_axis_sensor<float> {

public:
    /**
     * Conversion rate from degrees to radians
     */
    static constexpr float DEG_TO_RAD = M_PIf / 180.f;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif