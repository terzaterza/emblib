#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "task.h"

#include <functional>

namespace emblib::rtos::freertos {

/**
 * Possible FreeRTOS scheduler states
 */
enum class scheduler_state_e {
    SUSPENDED   = taskSCHEDULER_SUSPENDED,
    NOT_STARTED = taskSCHEDULER_NOT_STARTED,
    RUNNING     = taskSCHEDULER_RUNNING,
};

/**
 * Start FreeRTOS scheduler
*/
static inline void start_scheduler() noexcept
{
    vTaskStartScheduler();
}

/**
 * Return the scheduler state
 */
static inline scheduler_state_e get_scheduler_state() noexcept
{
    return static_cast<scheduler_state_e>(xTaskGetSchedulerState());
}

/**
 * Switch to a higher priority task if one was woken
 */
static inline void yield_from_isr(bool task_woken) noexcept
{
    portYIELD_FROM_ISR(task_woken);
}

/**
 * Delay the current task
 * @todo Can move to namespace this_task (this_thread)
 */
static inline void delay(time::tick ticks) noexcept
{
    vTaskDelay(ticks.count());
}

static inline uint32_t notify_take(bool clear_count, time::tick ticks) noexcept
{
    return ulTaskNotifyTake(clear_count, ticks.count());
}

/**
 * Stack buffer for a FreeRTOS task, where `SIZE` is the
 * number of words for the allocation
 * @todo Can move this inside task class and rename to "stack"
 */
template <size_t SIZE_WORDS>
using task_stack_t = StackType_t[SIZE_WORDS];

/**
 * FreeRTOS Task
 */
class task {

public:
    template <size_t STACK_SIZE>
    explicit task(
        std::function<void ()> task_func,
        const char* name,
        size_t priority,
        task_stack_t<STACK_SIZE>& stack
    ) :
        m_task_func(task_func),
        m_task_handle(xTaskCreateStatic(
            task_entry,
            name,
            STACK_SIZE,
            this,
            priority,
            stack,
            m_task_buffer
        ))
    {}

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Increment task's notification value (works like a counting semaphore)
     */
    void notify() noexcept
    {
        xTaskNotifyGive(m_task_handle);
    }

    /**
     * Increment task's notification value
     * @todo Add argument for higher priority task woken
     */
    void notify_from_isr() noexcept
    {
        vTaskNotifyGiveFromISR(m_task_handle, NULL);
    }

private:
    /**
     * Actual function which is called by the scheduler which then calls the
     * appropriate cpp style task function to allow for passing contexts to
     * the thread using std::function
     */
    static void task_entry(task* instance) noexcept
    {
        instance->m_task_func();

        /* If the function ever exits, remove this task */
        vTaskDelete(instance->m_task_handle);
    }

private:
    std::function<void ()> m_task_func;
    
    StaticTask_t m_task_buffer;
    TaskHandle_t m_task_handle;
    // TickType_t m_delay_until_last = 0;
};

}