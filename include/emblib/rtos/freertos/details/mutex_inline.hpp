#pragma once

inline bool mutex::lock(ticks_t timeout) noexcept
{
    return m_native_mutex.take(timeout.count());
}

inline bool mutex::unlock() noexcept
{
    return m_native_mutex.give();
}