#include "emblib/drivers/serial_device.hpp"

namespace emblib::drivers {

ssize_t serial_device::write(const char* data, size_t size) noexcept
{
    ssize_t ret_status = -1;

    /* Wait for lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->write_handler(data, size);
        assert(this->mutex.unlock() == status::OK);
    }
    return ret_status;
}


ssize_t serial_device::read(char* buffer, size_t size) noexcept
{
    ssize_t ret_status = -1;

    /* Wait for lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->read_handler(buffer, size);
        assert(this->mutex.unlock() == status::OK);
    }
    return ret_status;
}


bool serial_device::write_async(const char* data, size_t size) noexcept
{
    bool ret_status = false;

    /* Wait for lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->write_async_handler(data, size);
        assert(this->mutex.unlock() == status::OK);
    }
    return ret_status;
}


bool serial_device::write_async(const char* data, size_t size, rtos::mutex*& mutex_ptr) noexcept
{
    bool ret_status = false;

    /* Wait for lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->write_async_handler(data, size);

        if (ret_status) {
            mutex_ptr = &this->mutex;
        } else {
            assert(this->mutex.unlock() == status::OK);
        }
    }
    return ret_status;
}


bool serial_device::read_async(char* buffer, size_t size) noexcept
{
    bool ret_status = false;

    /* Wait for lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->read_async_handler(buffer, size);
        assert(this->mutex.unlock() == status::OK);
    }
    return ret_status;
}


bool serial_device::read_async(char* buffer, size_t size, rtos::mutex*& mutex_ptr) noexcept
{
    bool ret_status = false;

    /* Wait for lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->read_async_handler(buffer, size);
        
        if (ret_status) {
            mutex_ptr = &this->mutex;
        } else {
            assert(this->mutex.unlock() == status::OK);
        }
    }
    return ret_status;
}


bool serial_device::probe() noexcept
{
    bool ret_status = false;

    /* Lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->probe_handler();
        assert(this->mutex.unlock() == status::OK);
    }
    return ret_status;
}

}