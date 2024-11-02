#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/time.hpp"

#include "FreeRTOS.h"
#include "queue.h"

namespace emblib::rtos::freertos {

template <typename item_type, size_t CAPACITY>
class queue {

public:
    explicit queue() noexcept :
        m_queue_handle(xQueueCreateStatic(CAPACITY, sizeof(item_type), m_storage, &m_queue_buffer))
    {}

    /* Copy operations not allowed */
    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;

    /* Move operations not allowed */
    queue(queue&&) = delete;
    queue& operator=(queue&&) = delete;

    /**
     * Queue send
     */
    bool send(const item_type& item, time::tick timeout) noexcept
    {
        return xQueueSend(m_queue_handle, &item, timeout.count()) == pdTRUE;
    }

    /**
     * Queue send from ISR
     */
    bool send_from_isr(const item_type& item) noexcept
    {
        return xQueueSendFromISR(m_queue_handle, &item, NULL) == pdTRUE;
    }

    /**
     * Receive item from queue
     */
    bool receive(item_type& buffer, time::tick timeout) noexcept
    {
        return xQueueReceive(m_queue_handle, &buffer, timeout.count()) == pdTRUE;
    }

    /**
     * Peek queue
     */
    bool peek(item_type& buffer, time::tick timeout) noexcept
    {
        return xQueuePeek(m_queue_handle, &buffer, timeout.count()) == pdTRUE;
    }

private:
    QueueHandle_t m_queue_handle;
    StaticQueue_t m_queue_buffer;

    uint8_t m_storage[CAPACITY * sizeof(item_type)];

};

}