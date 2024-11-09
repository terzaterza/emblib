#pragma once

#define EMBLIB_CHAR_DEV_SUPPORT_ETL 1

#define EMBLIB_TIME_ALIGN_MILLIS    1
#define EMBLIB_TIME_TICK_MILLIS     1

#define EMBLIB_RTOS_USE_FREERTOS    1
#define EMBLIB_RTOS_USE_THREADX     0

#define EMBLIB_I2C_7BIT_ADDRESS     1

#define EMBLIB_MATH_USE_GLM         0
#define EMBLIB_MATH_USE_EIGEN       0
#define EMBLIB_MATH_USE_XTENSOR     1

namespace emblib {

using size_t = unsigned;

/**
 * Logger buffer size
 */
static constexpr size_t LOGGER_BUFFER_SIZE = 64;

/**
 * Capacity for the output channel wrapper class vector
 */
static constexpr size_t I2C_MUX_MAX_CHANNELS = 8;

}