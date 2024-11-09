#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/vector.hpp"

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
     * Read data of a single axis into the `out_g` argument in standard gravity [g]
     */
    virtual bool read_axis(axis_e axis, float& out_g) noexcept = 0;

    /**
     * Read data of all axes into into a 3 dimensional vector in standard gravity [g]
     * @note Default implementation is to read all 3 axes separately and combine the result
     * into a vector, but can be overriden to allow reading all the values in one bus transaction
     */
    virtual bool read(math::vector3f& out_vec) noexcept
    {
        bool success = true;
        success &= read_axis(axis_e::X, out_vec(0));
        success &= read_axis(axis_e::Y, out_vec(1));
        success &= read_axis(axis_e::Z, out_vec(2));
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