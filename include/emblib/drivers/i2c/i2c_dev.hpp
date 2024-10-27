#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/char_dev.hpp"
#include "./i2c_master.hpp"

namespace emblib::drivers {

class i2c_dev : public char_dev {

public:
    explicit i2c_dev(i2c_master& i2c_bus, i2c_address_t address)
        : m_i2c_bus(i2c_bus), m_address(address) {}

    /**
     * Write an array of bytes
     * @returns `-1` if error, else number of bytes written
     * @note Exits once the write operation is complete
    */
    ssize_t write(const char* data, size_t size) noexcept override
    {
        return m_i2c_bus.write(m_address, data, size);
    }

    /**
     * Read up to `size` bytes into the buffer
     * @returns `-1` if error, else number of bytes read
     * @note Exits once the read operation is complete
    */
    ssize_t read(char* buffer, size_t size) noexcept override
    {
        return m_i2c_bus.read(m_address, buffer, size);
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
    i2c_master& m_i2c_bus;
};

}