#pragma once

#include "emblib/emblib.hpp"
#include "emblib/driver/io/char_dev.hpp"
#include "i2c_bus.hpp"

namespace emblib::driver {

/**
 * I2C Device
 * @todo Can be final
 */
class i2c_dev : public char_dev {

public:
    explicit i2c_dev(i2c_bus& i2c_bus, i2c_address_t address)
        : m_i2c_bus(i2c_bus), m_address(address) {}

    /**
     * Write an array of bytes
     * @returns `-1` if error, else number of bytes written
     * @note Exits once the write operation is complete
    */
    ssize_t write(const char* data, size_t size, milliseconds timeout) noexcept override
    {
        return m_i2c_bus.write(m_address, data, size, timeout);
    }

    /**
     * Read up to `size` bytes into the buffer
     * @returns `-1` if error, else number of bytes read
     * @note Exits once the read operation is complete
    */
    ssize_t read(char* buffer, size_t size, milliseconds timeout) noexcept override
    {
        return m_i2c_bus.read(m_address, buffer, size, timeout);
    }

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     */
    bool write_async(const char* data, size_t size, const callback_t cb = callback_t()) noexcept override
    {
        return m_i2c_bus.write_async(m_address, data, size, cb);
    }

    /**
     * Start an async read
     * @return `true` if write operation started successfully
     */
    bool read_async(char* buffer, size_t size, const callback_t cb = callback_t()) noexcept override
    {
        return m_i2c_bus.read_async(m_address, buffer, size, cb);
    }

    /** @todo Add probe override */

    /**
     * @return `true` if current serial device supports async operations
     */
    bool is_async_available() noexcept override
    {
        return m_i2c_bus.is_async_available();
    }

private:
    i2c_address_t m_address;
    i2c_bus& m_i2c_bus;
};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif