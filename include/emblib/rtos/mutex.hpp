#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
#include "emblib/rtos/freertos/mutex_base.hpp"
#elif EMBLIB_RTOS_USE_THREADX
#include "emblib/rtos/threadx/mutex_base.hpp"
#else
#include "emblib/rtos/baremetal/mutex_base.hpp"
#endif

namespace emblib::rtos {

class mutex : private mutex_base {

public:
    mutex() = default;
    ~mutex() = default;

    /* Copy operations not allowed */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /* Move operations not allowed */
    mutex(mutex&&) = delete;
    mutex& operator=(mutex&&) = delete;

    /**
     * Mutex lock
     * @todo Add desc
    */
    status lock(time::tick ticks)
    {
        return mutex_base::lock(ticks);
    }

    /**
     * Mutex unlock
     * @todo Add desc
    */
    status unlock()
    {
        return mutex_base::unlock();
    }

};

}