#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
#include "emblib/rtos/freertos/mutex_base.hpp"
using namespace emblib::rtos::freertos;
#elif EMBLIB_RTOS_USE_THREADX
#include "emblib/rtos/threadx/mutex_base.hpp"
using namespace emblib::rtos::threadx;
#else
#include "emblib/rtos/baremetal/mutex_base.hpp"
using namespace emblib::rtos::baremetal;
#endif

namespace emblib::rtos {

class mutex : public mutex_base {

public:
    mutex() = default;
    ~mutex() = default;

    /* Copy operations not allowed */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /* Move operations not allowed */
    mutex(mutex&&) = delete;
    mutex& operator=(mutex&&) = delete;

};

}