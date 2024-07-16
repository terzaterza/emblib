#pragma once

#include "emblib/emblib.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
#include "emblib/rtos/freertos/queue.hpp"
#elif EMBLIB_RTOS_USE_THREADX
#include "emblib/rtos/threadx/queue.hpp"
#else
#include "emblib/rtos/baremetal/queue.hpp"
#endif

namespace emblib::rtos {

#if EMBLIB_RTOS_USE_FREERTOS
using freertos::queue;
#elif EMBLIB_RTOS_USE_THREADX
using threadx::mutex;
#else
using namespace baremetal;
#endif

}