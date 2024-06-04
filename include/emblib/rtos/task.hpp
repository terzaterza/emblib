#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/common/time.hpp"

#if EMBLIB_RTOS_USE_FREERTOS
#include "emblib/rtos/freertos/task_base.hpp"
using namespace emblib::rtos::freertos;
#elif EMBLIB_RTOS_USE_THREADX
#include "emblib/rtos/threadx/task_base.hpp"
using namespace emblib::rtos::threadx;
#else
#include "emblib/rtos/baremetal/task_base.hpp"
using namespace emblib::rtos::baremetal;
#endif

namespace emblib::rtos {

template <size_t stack_words, typename params_type>
class task : public task_base<stack_words, params_type> {

public:
    task() = default;
    ~task() = default;

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

};

}