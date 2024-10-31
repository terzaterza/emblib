#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "semphr.h"

namespace emblib::rtos::freertos {

class mutex {

public:
    explicit mutex() noexcept :
        m_semaphore_handle(xSemaphoreCreateMutexStatic(&m_semaphore_buffer))
    {}

    /** @todo Can add non virtual destructors for task, mutex, queue which
     * delete the corresponding handle object */

    /* Copy operations not allowed */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /* Move operations not allowed */
    mutex(mutex&&) = delete;
    mutex& operator=(mutex&&) = delete;

    /**
     * Mutex take
     * @todo Can add checking if the scheduler has started and returning
     * `true` if not since that means there can be only 1 thread running
    */
    bool take(time::tick ticks) noexcept
    {
        return xSemaphoreTake(m_semaphore_handle, ticks.count()) == pdTRUE;
    }

    /**
     * Mutex give
     * @todo Similar to `take`, can return true if scheduler not started
    */
    bool give() noexcept
    {
        return xSemaphoreGive(m_semaphore_handle) == pdTRUE;
    }

    /**
     * Mutex give from interrupt routine
     * @note Use this to make sure that task which was
     * interrupted does not block accidentally
     * @todo Can add higher prio task woken
     */
    bool give_from_isr() noexcept
    {
        return xSemaphoreGiveFromISR(m_semaphore_handle, NULL) == pdTRUE;
    }

private:
    StaticSemaphore_t m_semaphore_buffer;
    SemaphoreHandle_t m_semaphore_handle;

};

}