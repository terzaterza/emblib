#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "semphr.h"

namespace emblib::rtos::freertos {

enum class semaphore_type_e {
    BINARY,
    COUNTING,
    MUTEX
};

class semaphore {

public:
    explicit semaphore(semaphore_type_e type, size_t max_count = UINT32_MAX, size_t initial_count = 0) noexcept
    {
        if (type == semaphore_type_e::BINARY) {
            m_semaphore_handle = xSemaphoreCreateBinaryStatic(&m_semaphore_buffer);
        } else if (type == semaphore_type_e::COUNTING) {
            m_semaphore_handle = xSemaphoreCreateCountingStatic(max_count, initial_count, &m_semaphore_buffer);
        } else {
            m_semaphore_handle = xSemaphoreCreateMutexStatic(&m_semaphore_buffer);
        }
    }

    /** @todo Can add non virtual destructors for task, semaphore, queue which
     * delete the corresponding handle object */

    /* Copy operations not allowed */
    semaphore(const semaphore&) = delete;
    semaphore& operator=(const semaphore&) = delete;

    /* Move operations not allowed */
    semaphore(semaphore&&) = delete;
    semaphore& operator=(semaphore&&) = delete;

    /**
     * Semaphore take
     * @todo Can add checking if the scheduler has started and returning
     * `true` if not since that means there can be only 1 thread running
    */
    bool take(time::tick ticks) noexcept
    {
        return xSemaphoreTake(m_semaphore_handle, ticks.count()) == pdTRUE;
    }

    /**
     * Semaphore give
     * @todo Similar to `take`, can return true if scheduler not started
    */
    bool give() noexcept
    {
        return xSemaphoreGive(m_semaphore_handle) == pdTRUE;
    }

    /**
     * Semaphore give from interrupt routine
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


class mutex : public semaphore {

public:
    explicit mutex() noexcept :
        semaphore(semaphore_type_e::MUTEX)
    {}

};

}