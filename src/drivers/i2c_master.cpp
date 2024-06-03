#include "emblib/drivers/i2c_master.hpp"

namespace emblib::drivers {

status i2c_master::write(const i2c_address address, const uint8_t* data, const size_t size)
{
    status ret_status = status::ERROR;

    /** @todo Log i2c master write start (address, data, size) */

    if (this->mutex.lock(rtos::MUTEX_MAX_TICKS) == status::OK) {
        ret_status = this->write_handler(address, data, size);
        assert(this->mutex.unlock() == status::OK);
    }

    /** @todo If lock failed, log mutex lock fail */
    /** @todo Log ret_status */

    return ret_status;
}


status i2c_master::read(const i2c_address address, uint8_t* buffer, const size_t size)
{
    status ret_status = status::ERROR;

    /** @todo Log i2c master read start */

    if (this->mutex.lock(rtos::MUTEX_MAX_TICKS) == status::OK) {
        ret_status = this->read_handler(address, buffer, size);
        assert(this->mutex.unlock() == status::OK);
    }

    /** @todo If lock failed, log mutex lock fail */
    /** @todo Log ret_status */

    return ret_status;
}


status i2c_master::probe(const i2c_address address)
{
    status ret_status = status::ERROR;

    /** @todo Log i2c master probe start */

    if (this->mutex.lock(rtos::MUTEX_MAX_TICKS) == status::OK) {
        ret_status = this->probe_handler(address);
        assert(this->mutex.unlock() == status::OK);
    }

    /** @todo If lock failed, log mutex lock fail */
    /** @todo Log ret_status */

    return ret_status;
}


status i2c_master::reset()
{
    status ret_status = status::ERROR;

    /** @todo Log i2c master write start */

    if (this->mutex.lock(rtos::MUTEX_MAX_TICKS) == status::OK) {
        ret_status = this->reset_handler();
        assert(this->mutex.unlock() == status::OK);
    }

    /** @todo If lock failed, log mutex lock fail */
    /** @todo Log ret_status */

    return ret_status;
}


}