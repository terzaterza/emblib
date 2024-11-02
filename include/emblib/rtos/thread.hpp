#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/time.hpp"

#include <functional>

#if EMBLIB_RTOS_USE_FREERTOS
#include "./freertos/task.hpp"
#else
#error "Thread implementation missing"
#endif

namespace emblib::rtos {

/**
 * Namespace for all functions which implicitly reference the currently running thread
 */
namespace this_thread {

/**
 * Put this thread to sleep for a given time
 */
static void sleep(time::millisec duration) noexcept;


#if EMBLIB_RTOS_SUPPORT_NOTIFICATIONS
/**
 * Wait for this thread to get notified
 * @returns `false` on timeout, else `true`
 */
static bool wait_notification(time::millisec timeout) noexcept;
#endif

}

/**
 * Starts the RTOS scheduler
 * @todo If baremetal do nothing
 */
static void start_threads() noexcept;


/* Native (implementation based) type for underlying thread object */
// class native_thread_t;
// template <size_t SIZE>
// class native_stack_t;

/**
 * Thread abstraction
 */
class thread {

public:
    template <size_t STACK_SIZE>
    explicit thread(
        std::function<void ()> thread_func,
        const char* name,
        size_t priority,
        native_stack_t<STACK_SIZE>& stack
    );

    /* Copy operations not allowed */
    thread(const thread&) = delete;
    thread& operator=(const thread&) = delete;

    /* Move operations not allowed */
    thread(thread&&) = delete;
    thread& operator=(thread&&) = delete;


#if EMBLIB_RTOS_SUPPORT_NOTIFICATIONS
    /**
     * Increment thread's notification value
     * @note Unblocks this thread if is currently waiting on notification
     */
    void notify(time::millisec timeout) noexcept;
#endif

    /**
     * Get reference to the underlying implementation object
     */
    native_thread_t& get_native_thread() noexcept
    {
        return m_native_thread;
    }

private:
    native_thread_t m_native_thread;

};

/**
 * Implementation of thread functionality using FreeRTOS
 */
#if EMBLIB_RTOS_USE_FREERTOS

static void this_thread::sleep(time::millisec duration) noexcept
{
    freertos::delay(duration);
}

#if EMBLIB_RTOS_SUPPORT_NOTIFICATIONS
static bool this_thread::wait_notification(time::millisec timeout) noexcept
{
    return freertos::notify_take(false, timeout);
}

inline void thread::notify(time::millisec timeout) noexcept
{
    m_native_thread.notify();
}
#endif

static void start_threads() noexcept
{
    freertos::start_scheduler();
}


using native_thread_t = freertos::task;
template <size_t SIZE>
using native_stack_t = freertos::task_stack<SIZE>;

template <size_t STACK_SIZE>
inline thread::thread(
    std::function<void ()> thread_func,
    const char* name,
    size_t priority,
    native_stack_t<STACK_SIZE>& stack) :
    m_native_thread(thread_func, name, priority, stack)
{
}

#else
#error "Missing implementation of rtos::thread"
#endif

}