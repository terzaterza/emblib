#pragma once

#define EMBLIB_UNNEST_NAMESPACES    0

#define EMBLIB_CHAR_DEV_SUPPORT_ETL 1

#define EMBLIB_RTOS_USE_FREERTOS    1
#define EMBLIB_RTOS_USE_THREADX     0
#define EMBLIB_RTOS_TICK_MILLIS     1
#define EMBLIB_RTOS_SUPPORT_NOTIFICATIONS 1

#define EMBLIB_MATH_USE_GLM         0
#define EMBLIB_MATH_USE_EIGEN       1

namespace emblib {

/**
 * Capacity for the output channel wrapper class vector
 */
static constexpr size_t I2C_MUX_MAX_CHANNELS = 8;

}