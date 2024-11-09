#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::driver::sensor {

/**
 * Calculate bias by finding the average of `n` readings
 * @note Requires passing a delay function to make a pause between readings
 * @returns true if all readings were successful and bias calculated
 */
template <typename sensor_type>
inline bool calculate_bias(math::vector3f& bias, size_t n, std::function<void()> read_delay) noexcept
{
    bias.fill(0);
    
    math::vector3f read_data(0);
    for (size_t i = 0; i < n; i++) {
        if (!read(read_data)) {
            return false;
        }

        bias += data;
        read_delay();
    }

    bias /= static_cast<float>(n);
    return true;
}

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver::sensor;
}
#endif