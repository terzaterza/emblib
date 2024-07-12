#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
#include "emblib/rtos/freertos/task.hpp"
#elif EMBLIB_RTOS_USE_THREADX
#include "emblib/rtos/threadx/task.hpp"
#else
#include "emblib/rtos/baremetal/task.hpp"
#endif

namespace emblib::rtos {

#if EMBLIB_RTOS_USE_FREERTOS
template <size_t stack_words, typename params_t>
using task_base = freertos::task<stack_words, params_t>;
#elif EMBLIB_RTOS_USE_THREADX
using threadx::task_base;
#else
using baremetal::task_base;
#endif

template <size_t stack_words, typename params_t>
class task : public task_base<stack_words, params_t> {

public:
    /* Inherit the constructor from the base class */
    using task_base<stack_words, params_t>::task_base;

    /* Default (empty) destructor */
    ~task() = default;

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

};

}