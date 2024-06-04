#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "semphr.h"

namespace emblib::rtos::freertos {

constexpr time::tick MUTEX_MAX_TICKS{portMAX_DELAY};

class mutex_base {

public:

    /**
     * Mutex take
    */
    status lock(time::tick ticks = MUTEX_MAX_TICKS) noexcept
    {
        BaseType_t ret_status = xSemaphoreTake(semaphore_handle, ticks.count());
        return ret_status == pdTRUE ? status::OK : status::ERROR;
    }

    /**
     * Mutex give
    */
    status unlock() noexcept
    {
        BaseType_t ret_status = xSemaphoreGive(semaphore_handle);
        return ret_status == pdTRUE ? status::OK : status::ERROR;
    }

protected:
    explicit mutex_base() noexcept
        : semaphore_handle(xSemaphoreCreateMutexStatic(&semaphore_buffer)) {}

    virtual ~mutex_base() = default;

private:
    StaticSemaphore_t semaphore_buffer;
    SemaphoreHandle_t semaphore_handle;

};

}