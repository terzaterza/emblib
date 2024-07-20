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
 * FreeRTOS Task
 */
template <size_t stack_words, typename params_t = void>
class task {

public:
    explicit task(const char* name, size_t priority) noexcept :
        name(name),
        priority(priority)
    {}

    virtual ~task() = default;

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Delay currently running task
     */
    static void delay(time::tick ticks) noexcept
    {
        vTaskDelay(ticks.count());
    }

    /**
     * Start the task (put in ready state)
     */
    void start(params_t* params = nullptr) noexcept
    {
        this->params.instance = this;
        this->params.params = params;
        task_handle = xTaskCreateStatic(
            reinterpret_cast<void (*)(void*)>(task_thread),
            name,
            stack_words > configMINIMAL_STACK_SIZE ? stack_words : configMINIMAL_STACK_SIZE,
            &this->params,
            priority,
            stack_buffer,
            &task_buffer
        );
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
    status wait_notify(time::tick ticks = time::tick(portMAX_DELAY), bool clear = true) noexcept
    {
        uint32_t count = ulTaskNotifyTake(clear, ticks.count());
        return count > 0 ? status::OK : status::ERROR;
    }

private:
    /**
     * Information for the thread about the task and user defined parameters
     */
    struct thread_params {
        task* instance;
        params_t* params;
    };

    /**
     * Actual function which is called by the scheduler which then calls the
     * appropriate task method (task::run override) to allow for instance
     * specific task threads (allows using of `this` inside task function)
     */
    static void task_thread(thread_params* params) noexcept
    {
        params->instance->run(params->params);
    }

    /**
     * Task function
     * @note Should never return
     * @todo Add [[noreturn]]
    */
    virtual void run(params_t* params) noexcept = 0;

private:
    StackType_t stack_buffer[stack_words];
    StaticTask_t task_buffer;
    TaskHandle_t task_handle;

    TickType_t delay_until_last = 0;

    const char* name;
    size_t priority;
    thread_params params;
};

}