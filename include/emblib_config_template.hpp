#pragma once

#define EMBLIB_TIME_ALIGN_MILLIS    1
#define EMBLIB_TIME_TICK_MILLIS     1

#define EMBLIB_RTOS_USE_FREERTOS    1
#define EMBLIB_RTOS_USE_THREADX     0
#define EMBLIB_RTOS_USE_BAREMETAL   !(EMBLIB_RTOS_USE_FREERTOS || EMBLIB_RTOS_USE_THREADX)

#define EMBLIB_I2C_7BIT_ADDRESS     1

#define EMBLIB_MATH_USE_GLM         0
#define EMBLIB_MATH_USE_EIGEN       1

namespace emblib {

/**
 * Logger buffer size
 */
static constexpr size_t LOGGER_BUFFER_SIZE = 64;

}