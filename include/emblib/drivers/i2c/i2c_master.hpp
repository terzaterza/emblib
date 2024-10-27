#pragma once

#include "emblib/emblib.hpp"
#include <functional>

namespace emblib::drivers {

/**
 * @todo If needed replace this with a class i2c_address which
 * can then support both 7 and 10 bit addresses
 */
typedef uint16_t i2c_address_t;

/**
 * I2C master base class
 * 
 * @todo Currently not supporting 10 bit addresses, but can
 * add this property to constructor parameters
 */
class i2c_master {

public:
    /* Private typedef for async operation callbacks */
    using callback_t = std::function<void(ssize_t)>;

    explicit i2c_master() = default;
    virtual ~i2c_master() = default;

    /* Copy operations not allowed */
    i2c_master(const i2c_master&) = delete;
    i2c_master& operator=(const i2c_master&) = delete;

    /* Move operations not allowed */
    i2c_master(i2c_master&&) = delete;
    i2c_master& operator=(i2c_master&&) = delete;

    /**
     * Write an array of bytes to the device with the specified address
     * @returns `-1` if error, else number of bytes written
     * @note Exits once the write operation is complete
    */
    virtual ssize_t write(i2c_address_t address, const char* data, size_t size) noexcept = 0;

    /**
     * Read up to `size` bytes into the buffer from the device with the specified address
     * @returns `-1` if error, else number of bytes read
     * @note Exits once the read operation is complete
    */
    virtual ssize_t read(i2c_address_t address, char* buffer, size_t size) noexcept = 0;

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     */
    virtual bool write_async(i2c_address_t address, const char* data, size_t size, const callback_t cb = callback_t()) noexcept = 0;

    /**
     * Start an async read
     * @return `true` if write operation started successfully
     */
    virtual bool read_async(i2c_address_t address, char* buffer, size_t size, const callback_t cb = callback_t()) noexcept = 0;

    /**
     * Tests if the device is responding
     * @returns `true` if device responds
     * @note Default implementation is a dummy read
    */
    virtual bool probe(i2c_address_t address) noexcept
    {
        return read(address, nullptr, 0) == 0;
    }

    /**
     * @return `true` if current serial device supports async operations
     */
    virtual bool is_async_available() noexcept
    {
        return false;
    }

    /**
     * Resets the I2C master object state and releases the bus if it is held
     * @note Constructors should use this function to initialize the device
    */
    bool reset();

};

}