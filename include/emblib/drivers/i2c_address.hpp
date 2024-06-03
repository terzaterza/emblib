#pragma once

#include "emblib/emblib.hpp"

namespace emblib::drivers {

#if EMBLIB_I2C_7BIT_ADDRESS

typedef uint8_t i2c_address;

#else

typedef uint16_t i2c_address;

#endif

}