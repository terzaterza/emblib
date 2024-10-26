#pragma once

#include "emblib/emblib.hpp"
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
    bool send(const item_t& item, time::tick ticks_wait = time::tick{portMAX_DELAY}) noexcept
    {
        return xQueueSend(queue_handle, &item, ticks_wait.count()) == pdTRUE;
    }

    /**
     * Queue send overwrite
     * @todo Should merge into send(const item_t& item, bool overwrite = false, time::tick ...);
     */
    bool send_overwrite(const item_t& item) noexcept
    {
        static_assert(capacity == 1); /* Required by FreeRTOS */
        return xQueueOverwrite(queue_handle, &item) == pdTRUE;
    }

    /**
     * Queue send from ISR
     */
    bool send_from_isr(const item_t& item, bool* task_woken = nullptr) noexcept
    {
        BaseType_t task_woken_base = pdFALSE;
        bool ret_status = xQueueSendFromISR(queue_handle, &item, &task_woken_base) == pdTRUE;

        if (task_woken) {
            *task_woken = task_woken_base == pdTRUE;
        }
        return ret_status;
    }

    /**
     * Receive item from queue
     */
    bool receive(item_t* buffer, time::tick ticks_wait = time::tick{portMAX_DELAY}) noexcept
    {
        return xQueueReceive(queue_handle, buffer, ticks_wait.count()) == pdTRUE;
    }

    /**
     * Receive item from queue without deleting it
     */
    bool peek(item_t* buffer, time::tick ticks_wait = time::tick{portMAX_DELAY}) noexcept
    {
        return xQueuePeek(queue_handle, buffer, ticks_wait.count()) == pdTRUE;
    }

private:
    QueueHandle_t queue_handle;
    StaticQueue_t queue_buffer;

    uint8_t storage[capacity * sizeof(item_t)];

};

}