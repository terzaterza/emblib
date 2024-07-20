#include "emblib/drivers/serial_device.hpp"

namespace emblib::drivers {

status serial_device::write(const char* data, size_t size) noexcept
{
    status ret_status = status::ERROR;

    /* Lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->write_handler(data, size);
        assert(this->mutex.unlock() == status::OK);
    }
    return ret_status;
}


status serial_device::read(char* buffer, size_t size) noexcept
{
    status ret_status = status::ERROR;

    /* Lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->read_handler(buffer, size);
        assert(this->mutex.unlock() == status::OK);
    }
    return ret_status;
}


status serial_device::write_async(const char* data, size_t size) noexcept
{
    status ret_status = status::ERROR;

    /* Lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->write_async_handler(data, size);
        if (ret_status != status::OK) {
            assert(this->mutex.unlock() == status::OK);
        }
    }
    return ret_status;
}


status serial_device::read_async(char* buffer, size_t size) noexcept
{
    status ret_status = status::ERROR;

    /* Lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->read_async_handler(buffer, size);
        if (ret_status != status::OK) {
            assert(this->mutex.unlock() == status::OK);
        }
    }
    return ret_status;
}


status serial_device::probe() noexcept
{
    status ret_status = status::ERROR;

    /* Lock indefinetely by default */
    if (this->mutex.lock() == status::OK) {
        ret_status = this->probe_handler();
        assert(this->mutex.unlock() == status::OK);
    }
    return ret_status;
}

}