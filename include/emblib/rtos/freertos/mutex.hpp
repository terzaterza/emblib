#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

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
    */
    status lock(time::tick ticks = time::tick{portMAX_DELAY}) noexcept
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

private:
    StaticSemaphore_t semaphore_buffer;
    SemaphoreHandle_t semaphore_handle;

};

}