#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "task.h"

namespace emblib::rtos::freertos {

template <size_t stack_words, typename params_t>
class task {

public:
    explicit task(const char* name, size_t priority, const params_t& params) noexcept
        : task_params(params), task_handle(xTaskCreateStatic(
            run,
            name,
            stack_words,
            &task_params, priority,
            stack_buffer,
            &task_buffer
        )) {}

    virtual ~task() = default;

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Start FreeRTOS scheduler
    */
    static void start_scheduler() noexcept
    {
        vTaskStartScheduler();
    }

    /**
     * Delay currently running task
     */
    static void delay(time::tick ticks) noexcept
    {
        vTaskDelay(ticks.count());
    }

    /**
     * Increase task notification value and unblock task if is currently waiting
    */
    void notify() noexcept
    {
        xTaskNotifyGive(task_handle);
    }

protected:
    /**
     * Task will resume after `ticks` since the last time this function was called
     * @returns `true` if the task execution was delayed, else `false`
    */
    bool delay_until(time::tick ticks) noexcept
    {
        if (this->delay_until_last == 0) {
            this->delay_until_last = xTaskGetTickCount();
        }

        return xTaskDelayUntil(&this->delay_until_last, ticks.count());
    }

    /**
     * Wait for notification
     * @note If clear is true, than this wait consumes all notifications which are given to this task
     * @returns `status::ERROR` if ticks expired before notification was received
    */
    status wait_notify(time::ticks ticks, bool clear = true) noexcept
    {
        uint32_t count = ulTaskNotifyTake(clear, ticks.count());
        return count > 0 ? status::OK : status::ERROR;
    }

private:
    StackType_t stack_buffer[stack_words];
    StaticTask_t task_buffer;
    TaskHandle_t task_handle;
    params_t task_params;

    TickType_t delay_until_last = 0;

    /**
     * Task function
     * @note Should never return
    */
    virtual void run(params_t* params) noexcept = 0;

};

}