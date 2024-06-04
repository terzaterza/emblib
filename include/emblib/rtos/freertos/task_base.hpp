#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "task.h"

namespace emblib::rtos::freertos {

template <size_t stack_words, typename params_type>
class task_base {

public:
    /**
     * Increase task notification value and unblock task if is currently waiting
    */
    void notify() noexcept
    {
        xTaskNotifyGive(task_handle);
    }

protected:
    explicit task_base(const char* name, size_t priority, params_type& params) noexcept
        : task_handle(xTaskCreateStatic(
            task_function,
            name,
            stack_words,
            &params, priority,
            stack_buffer,
            &task_buffer
        )) {}

    virtual ~task_base() = default;

    /**
     * Wait for `ticks` before resuming
    */
    void delay(time::tick ticks) noexcept
    {
        vTaskDelay(ticks.count());
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

    virtual void task_function(params_type* params) = 0;

};

}