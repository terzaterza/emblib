#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"
#include "emblib/rtos/freertos/task.hpp"

#include "FreeRTOS.h"
#include "semphr.h"

namespace emblib::rtos::freertos {

class mutex {

public:
    explicit mutex() noexcept :
        semaphore_handle(xSemaphoreCreateMutexStatic(&semaphore_buffer))
    {}

    virtual ~mutex() = default;

    /* Copy operations not allowed */
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    /* Move operations not allowed */
    mutex(mutex&&) = delete;
    mutex& operator=(mutex&&) = delete;

    /**
     * Mutex take
     * @note If the scheduler is not yet started, this returns `status::OK`
     * without modifying the mutex since it is assumed there will be no
     * preemption until the scheduler starts
    */
    status lock(time::tick ticks = time::tick{portMAX_DELAY}) noexcept
    {
        if (get_scheduler_state() == scheduler_state::NOT_STARTED) {
            return status::OK;
        }

        BaseType_t ret_status = xSemaphoreTake(semaphore_handle, ticks.count());
        return ret_status == pdTRUE ? status::OK : status::ERROR;
    }

    /**
     * Mutex give
     * @note If the scheduler is not yet started, this returns `status::OK`
     * without modifying the mutex since it is assumed there will be no
     * preemption until the scheduler starts
    */
    status unlock() noexcept
    {
        if (get_scheduler_state() == scheduler_state::NOT_STARTED) {
            return status::OK;
        }
        
        BaseType_t ret_status = xSemaphoreGive(semaphore_handle);
        return ret_status == pdTRUE ? status::OK : status::ERROR;
    }

private:
    StaticSemaphore_t semaphore_buffer;
    SemaphoreHandle_t semaphore_handle;

};

}