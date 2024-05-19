#include "drivers/serial_device.hpp"

namespace emblib::drivers {


status serial_device::write(const uint8_t* data, const size_t size)
{
    status ret_status = status::ERROR;

    /** @todo Log serial device write start */

    if (this->mutex.lock(rtos::MUTEX_MAX_TICKS) == status::OK) {
        ret_status = this->write_handler(data, size);
        assert(this->mutex.unlock() == status::OK);
    }

    /** @todo If lock failed, log mutex lock fail */
    /** @todo Log ret_status */

    return ret_status;
}


status serial_device::read(uint8_t* buffer, const size_t size)
{
    status ret_status = status::ERROR;

    /** @todo Log serial device read start */

    if (this->mutex.lock(rtos::MUTEX_MAX_TICKS) == status::OK) {
        ret_status = this->read_handler(buffer, size);
        assert(this->mutex.unlock() == status::OK);
    }

    /** @todo If lock failed, log mutex lock fail */
    /** @todo Log ret_status */

    return ret_status;
}


status serial_device::probe()
{
    status ret_status = status::ERROR;

    /** @todo Log serial device read start */

    if (this->mutex.lock(rtos::MUTEX_MAX_TICKS) == status::OK) {
        ret_status = this->probe_handler();
        assert(this->mutex.unlock() == status::OK);
    }

    /** @todo If lock failed, log mutex lock fail */
    /** @todo Log ret_status */

    return ret_status;
}


}