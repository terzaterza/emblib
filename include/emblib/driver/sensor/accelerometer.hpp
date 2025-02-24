#pragma once

#include "emblib/emblib.hpp"
#include "three_axes_sensor.hpp"

namespace emblib::driver {

class accelerometer : public three_axes_sensor<float> {

public:
    /**
     * Conversion rate from standard gravity to meters per second^2
     */
    static constexpr float G_TO_MPS2 = 9.80665f;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif