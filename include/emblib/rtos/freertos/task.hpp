#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "task.h"

namespace emblib::rtos::freertos {

/**
 * Possible FreeRTOS scheduler states
 */
enum class scheduler_state {
    SUSPENDED   = 0,
    NOT_STARTED = 1,
    RUNNING     = 2,
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
static inline scheduler_state get_scheduler_state()
{
    return static_cast<scheduler_state>(xTaskGetSchedulerState());
}

/**
 * Switch to a higher priority task if one was woken
 */
static inline void yield_from_isr(bool task_woken) noexcept
{
    BaseType_t task_woken_base_type = task_woken ? pdTRUE : pdFALSE;
    portYIELD_FROM_ISR(task_woken_base_type);
}

/**
 * FreeRTOS Task
 * @todo Check if can remove this templating (external stack buffer)
 */
template <size_t stack_size>
class task {

public:
    /**
     * @note Stack size is in words
     */
    constexpr explicit task(const char* name, size_t priority) noexcept :
        task_handle (xTaskCreateStatic(
            reinterpret_cast<void (*)(void*)>(task_thread),
            name,
            stack_size > configMINIMAL_STACK_SIZE ? stack_size : configMINIMAL_STACK_SIZE,
            this,
            priority,
            stack_buffer,
            &task_buffer))
    {}

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Increase task notification value and unblock task if is currently waiting
    */
    void notify() noexcept
    {
        xTaskNotifyGive(task_handle);
    }

    /**
     * Increase task notification value and unblock task if is currently waiting
     * @todo Add higher priority task woken ptr as parameter
    */
    void notify_from_isr(bool* task_woken = nullptr) noexcept
    {
        BaseType_t task_woken_bt = pdFALSE;
        vTaskNotifyGiveFromISR(task_handle, &task_woken_bt);

        if (task_woken) {
            *task_woken = task_woken_bt == pdTRUE;
        }
    }

protected:
    /**
     * Delay currently running task
     */
    void delay(time::tick ticks) noexcept
    {
        vTaskDelay(ticks.count());
    }

    /**
     * Task will resume after `ticks` since the last time this function was called
     * @returns `true` if the task execution was delayed, else `false`
    */
    bool delay_until(time::tick ticks) noexcept
    {
        if (this->delay_until_last == 0) {
            this->delay_until_last = xTaskGetTickCount();
        }

        // BaseType_t was_delayed = xTaskDelayUntil(&this->delay_until_last, ticks.count());
        // return was_delayed == pdTRUE;
        vTaskDelayUntil(&this->delay_until_last, ticks.count());
        return true;
    }

    /**
     * Wait for notification
     * @note If clear is true, than this wait consumes all notifications which are given to this task
     * @returns `status::ERROR` if ticks expired before notification was received
    */
    status wait_notify(time::tick ticks = time::tick{portMAX_DELAY}, bool clear = false) noexcept
    {
        uint32_t count = ulTaskNotifyTake(clear, ticks.count());
        return count > 0 ? status::OK : status::ERROR;
    }

private:
    /**
     * Actual function which is called by the scheduler which then calls the
     * appropriate task method (task::run override) to allow for instance
     * specific task threads (allows using of `this` inside task function)
     */
    static void task_thread(task* instance) noexcept
    {
        instance->run();
    }

    /**
     * Task function
     * @note Should never return
     * @todo Add [[noreturn]]
    */
    virtual void run() noexcept = 0;

private:
    StackType_t stack_buffer[stack_size];
    StaticTask_t task_buffer;
    TaskHandle_t task_handle;
    TickType_t delay_until_last = 0;

};

}