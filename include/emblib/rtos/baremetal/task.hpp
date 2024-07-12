#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/time.hpp"

namespace emblib::rtos::baremetal {

class task {

public:
    static void delay(time::tick ticks) noexcept
    {
        /** @todo Implement as native::wait(ticks); */
    }

};

}