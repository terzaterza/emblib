#pragma once

template <typename item_type, size_t CAPACITY>
bool queue<item_type, CAPACITY>::send(const item_type& item, ticks_t timeout) noexcept
{
    return m_native_queue.send(item, timeout.count());
}

template <typename item_type, size_t CAPACITY>
bool queue<item_type, CAPACITY>::send_from_isr(const item_type& item) noexcept
{
    return m_native_queue.send_from_isr(item);
}

template <typename item_type, size_t CAPACITY>
bool queue<item_type, CAPACITY>::receive(item_type& buffer, ticks_t timeout) noexcept
{
    return m_native_queue.receive(buffer, timeout.count());
}

template <typename item_type, size_t CAPACITY>
bool queue<item_type, CAPACITY>::peek(item_type& buffer, ticks_t timeout) noexcept
{
    return m_native_queue.peek(buffer, timeout.count());
}