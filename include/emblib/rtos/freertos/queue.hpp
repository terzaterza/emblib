#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "queue.h"

namespace emblib::rtos::freertos {

template <typename item_t, size_t capacity>
class queue {

public:
    explicit queue() noexcept :
        queue_handle(xQueueCreateStatic(capacity, sizeof(item_t), storage, &queue_buffer))
    {}

    virtual ~queue() = default;

    /* Copy operations not allowed */
    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;

    /* Move operations not allowed */
    queue(queue&&) = delete;
    queue& operator=(queue&&) = delete;

    /**
     * Queue send
     */
    status send(const item_t& item, time::tick ticks_wait = time::tick{portMAX_DELAY}) noexcept
    {
        BaseType_t ret_status = xQueueSend(queue_handle, &item, ticks_wait.count());
        return ret_status == pdTRUE ? status::OK : status::ERROR;
    }

    /**
     * Queue send from ISR
     */
    status send_from_isr(const item_t& item, BaseType_t* high_prio_task_woken = 0) noexcept
    {
        BaseType_t ret_status = xQueueSendFromISR(queue_handle, &item, high_prio_task_woken);
        return ret_status == pdTRUE ? status::OK : status::ERROR;
    }

    /**
     * Receive item from queue
     */
    status receive(item_t* buffer, time::tick ticks_wait = time::tick{portMAX_DELAY}) noexcept
    {
        BaseType_t ret_status = xQueueReceive(queue_handle, buffer, ticks_wait.count());
        return ret_status == pdTRUE ? status::OK : status::ERROR;
    }

private:
    QueueHandle_t queue_handle;
    StaticQueue_t queue_buffer;

    uint8_t storage[capacity * sizeof(item_t)];

};

}