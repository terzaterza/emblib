#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
#include "emblib/rtos/freertos/task.hpp"
#elif EMBLIB_RTOS_USE_THREADX
#include "emblib/rtos/threadx/task.hpp"
#else
#include "emblib/rtos/baremetal/task.hpp"
#endif

namespace emblib::rtos {

#if EMBLIB_RTOS_USE_FREERTOS

using namespace freertos;

#elif EMBLIB_RTOS_USE_THREADX

using threadx::task_base;

#else

using baremetal::task_base;

#endif

}