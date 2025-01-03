#pragma once

template <size_t STACK_SIZE_BYTES>
inline task::task(const char *name, size_t priority, task_stack_t<STACK_SIZE_BYTES> &stack) :
    m_native_task([this] {this->run();}, name, priority, (freertos::task_stack_t<sizeof(stack) / sizeof(freertos::task_stack_t<1>)>&)stack)
{
}

inline void task::start_tasks() noexcept
{
    freertos::start_scheduler();
}

inline void task::sleep(ticks_t duration) noexcept
{
    vTaskDelay(duration.count());
}

inline void task::sleep_periodic(ticks_t period) noexcept
{
    m_native_task.sleep_periodic(period.count());
}

#if EMBLIB_RTOS_SUPPORT_NOTIFICATIONS
inline bool task::wait_notification(ticks_t timeout) noexcept
{
    return ulTaskNotifyTake(false, timeout.count());
}

inline void task::notify() noexcept
{
    m_native_task.notify();
}
#endif