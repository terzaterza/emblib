#include "emblib/rtos/freertos/task.hpp"

namespace emblib::rtos::freertos {

template <size_t stack_words, typename params_t>
void task<stack_words, params_t>::task_thread(thread_params* params) noexcept{
    params->instance->run(params->params);
}

}