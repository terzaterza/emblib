#pragma once

#include "emblib/emblib.hpp"

namespace emblib::driver {

class accelerometer {

public:
    /**
     * Conversion rate from standard gravity to meters per second^2
     */
    static constexpr float G_TO_MPS2 = 9.80665f;

    /* Used for read axis method */
    enum class axis_e {
        X, Y, Z
    };

public:
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
    virtual bool probe() noexcept = 0;

    /**
     * Check if there is new data available since the last read
     */
    virtual bool is_data_available() noexcept = 0;    

    /**
     * Read data of a single axis into the `out_g` argument in units of standard gravity [g]
     */
    virtual bool read_axis(axis_e axis, float& out_g) noexcept = 0;

    /**
     * Read data of all axes into an array in units of standard gravity [g]
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
     * Noise spectral density in units [g/sqrt(Hz)]
     */
    virtual float get_noise_density() const noexcept = 0;

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif