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
     * @todo Rename `ticks` to `timeout`
     */
    bool send(const item_type& item, time::tick ticks) noexcept
    {
        return xQueueSend(m_queue_handle, &item, ticks_wait.count()) == pdTRUE;
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
    bool receive(item_type& buffer, time::tick ticks) noexcept
    {
        return xQueueReceive(m_queue_handle, &buffer, ticks.count()) == pdTRUE;
    }

private:
    QueueHandle_t m_queue_handle;
    StaticQueue_t m_queue_buffer;

    uint8_t m_storage[CAPACITY * sizeof(item_type)];

};

}