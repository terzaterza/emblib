#pragma once

#include "emblib/emblib.hpp"

namespace emblib::driver {

template <typename data_type>
class three_axes_sensor {

public:
    /* Used for read axis method */
    enum class axis_e {
        X, Y, Z
    };

public:
    /**
     * Default constructor and destructor
    */
    explicit three_axes_sensor() = default;
    virtual ~three_axes_sensor() = default;

    /**
     * Check if the sensor identifies correctly
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
     * Read data of a single axis
     */
    virtual bool read_axis(axis_e axis, data_type& out) noexcept = 0;

    /**
     * Read data of all axes into an array
     * @note Default implementation is to read all 3 axes one by one, but
     * can be overriden to allow reading of all the values in one bus transaction
     */
    virtual bool read_all_axes(data_type (&out_data)[3]) noexcept
    {
        bool success = true;
        success &= read_axis(axis_e::X, out_data[0]);
        success &= read_axis(axis_e::Y, out_data[1]);
        success &= read_axis(axis_e::Z, out_data[2]);
        return success;
    }

    /**
     * Noise spectral density in units [data_type]/sqrt(Hz)
     */
    virtual float get_noise_density() const noexcept = 0;

    /// @todo Add public static methods for bias estimation and sphere fitting
    /// @todo Add methods for setting the data rate and the sensitivity (range)

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif