#pragma once

inline bool mutex::lock(time::millisec timeout) noexcept
{
    return m_native_mutex.take(timeout);
}

inline bool mutex::unlock() noexcept
{
    return m_native_mutex.give();
}