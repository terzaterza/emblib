#pragma once

#include "emblib.hpp"
#include "common/status.hpp"
#include "common/time.hpp"

namespace emblib::rtos {

class mutex_base {

protected:

    /**
     * Baremetal stub implementation for RTOS lock
     * @note Always returns `status::OK`
    */
    status lock(time::tick ticks)
    {
        return status::OK;
    }

    /**
     * Baremetal stub implementation for RTOS unlock
     * @note Always returns `status::OK`
    */
    status unlock()
    {
        return status::OK;
    }
    
};

}