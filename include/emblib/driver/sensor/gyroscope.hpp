#pragma once

#include "emblib/emblib.hpp"
#include <cmath>

namespace emblib::driver {

class gyroscope {

public:
    /**
     * Conversion rate from degrees to radians
     */
    static constexpr float DEG_TO_RAD = M_PIf / 180.f;
    
    /* Used for read axis method */
    enum class axis_e {
        X, Y, Z
    };

public:
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
    virtual bool probe() noexcept = 0;

    /**
     * Check if there is new data available since the last read
     */
    virtual bool is_data_available() noexcept = 0;    

    /**
     * Read data of a single axis into the `out_dps` argument in degrees per second [dps]
     */
    virtual bool read_axis(axis_e axis, float& out_dps) noexcept = 0;

    /**
     * Read data of all axes into an array in units of degrees per second [dps]
     * @note Default implementation is to read all 3 axes one by one, but
     * can be overriden to allow reading of all the values in one bus transaction
     */
    virtual bool read_all_axes(float (&out_data)[3]) noexcept
    {
        bool success = true;
        success &= read_axis(axis_e::X, out_data[0]);
        success &= read_axis(axis_e::Y, out_data[1]);
        success &= read_axis(axis_e::Z, out_data[2]);
        return success;
    }

    /**
     * Noise spectral density in units [dps/sqrt(Hz)]
     */
    virtual float get_noise_density() const noexcept = 0;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif