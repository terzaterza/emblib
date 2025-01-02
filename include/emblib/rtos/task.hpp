#pragma once

#include "emblib/emblib.hpp"
#if EMBLIB_RTOS_USE_FREERTOS
    #include "./freertos/task.hpp"
#else
#endif
#include <chrono>

namespace emblib::rtos {

/**
 * Statically allocated stack
 */
template <size_t SIZE_IN_BYTES>
using task_stack_t = uint8_t[SIZE_IN_BYTES];

/**
 * Duration of a period of time in number of ticks
 * @note Duration of a single tick is defined in emblib_config.hpp
 */
using ticks_t =
#if EMBLIB_RTOS_TICK_MILLIS
    std::chrono::milliseconds;
#else
    #error "Ticks not defined"
#endif

/**
 * Maximum duration used to signal indefinite waiting
 */
static constexpr ticks_t MAX_TICKS = ticks_t(-1);


/**
 * Thread interface
 */
class task {

public:
#if EMBLIB_RTOS_USE_FREERTOS
    using native_task_t = freertos::task;
#else
    #error "Task implementation missing"
#endif

public:
    template <size_t STACK_SIZE_BYTES>
    explicit task(
        const char* name,
        size_t priority,
        task_stack_t<STACK_SIZE_BYTES>& stack
    );
    virtual ~task() = default;

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Start the scheduler
     */
    static inline void start_tasks() noexcept;

    /**
     * Put the currently running thread to sleep
     * @note Static since can be called even baremetal and implemented using HAL
     */
    static inline void sleep(ticks_t duration) noexcept;

    /**
     * Put this task to sleep until (last wake up time from this method + period)
     * @note First time this is called, next wake up time is relative to task creation
     */
    void sleep_periodic(ticks_t period) noexcept;

#if EMBLIB_RTOS_SUPPORT_NOTIFICATIONS
    /**
     * Wait for this task to get notified
     * @note Lightweight version of a semaphore which can be
     * taken only by this task
     */
    bool wait_notification(ticks_t timeout = MAX_TICKS) noexcept;

    /**
     * Increment this task's notification value
     * @note Unblocks this task if is currently waiting on notification
     */
    void notify() noexcept;
#endif

    /**
     * Get reference to the underlying implementation object
     */
    native_task_t& get_native_task() noexcept
    {
        return m_native_task;
    }

private:
    /**
     * Task executable
     */
    virtual void run() noexcept = 0;

private:
    native_task_t m_native_task;

};


#if EMBLIB_RTOS_USE_FREERTOS
    #include "./freertos/details/task_inline.hpp"
#else
    #error "Thread implementation missing"
#endif

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace rtos;
}
#endif