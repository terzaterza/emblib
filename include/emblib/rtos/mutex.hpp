#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/time.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
#include "./freertos/semaphore.hpp"
#else
#error "Mutex implementation missing"
#endif


namespace emblib::rtos {

/**
 * Mutex
 * @note Can be used with std guards and locks
 */
class mutex {

public:
#if EMBLIB_RTOS_USE_FREERTOS
    using native_mutex_t = freertos::mutex;
#else
    #error "Thread implementation missing"
#endif

    explicit mutex() = default;

    /* Copy operations not allowed */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /* Move operations not allowed */
    mutex(mutex&&) = delete;
    mutex& operator=(mutex&&) = delete;

    /**
     * Mutex lock
     * 
     * @returns `true` if successful
     */
    bool lock(time::millisec timeout = time::MAX_MILLIS) noexcept;

    /**
     * Mutex unlock
     * 
     * @returns `true` if successful
     */
    bool unlock() noexcept;

    /**
     * Get reference to the underlying mutex object
     */
    native_mutex_t& get_native_mutex() noexcept
    {
        return m_native_mutex;
    }

private:
    native_mutex_t m_native_mutex;

};


#if EMBLIB_RTOS_USE_FREERTOS

inline bool mutex::lock(time::millisec timeout) noexcept
{
    return m_native_mutex.take(timeout);
}

inline bool mutex::unlock() noexcept
{
    return m_native_mutex.give();
}

#else
#error "Mutex implementation missing"
#endif

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace rtos;
}
#endif