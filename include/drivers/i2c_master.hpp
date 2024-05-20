#pragma once

#include "emblib.hpp"
#include "common/status.hpp"
#include "drivers/i2c_address.hpp"
#include "rtos/mutex.hpp"

namespace emblib::drivers {

class i2c_master {

public:
    explicit i2c_master() {}
    virtual ~i2c_master() = default;

    /* Copy operations not allowed */
    i2c_master(const i2c_master&) = delete;
    i2c_master& operator=(const i2c_master&) = delete;

    /* Move operations not allowed */
    i2c_master(i2c_master&&) = delete;
    i2c_master& operator=(i2c_master&&) = delete;

    /**
     * Write an array of bytes to the device with the specified address
     * @note This function acquires the I2C master mutex
     * @note Exits once the write operation is complete
    */
    status write(const i2c_address address, const uint8_t* data, const size_t size);

    /**
     * Read an array of bytes from the device with the specified address
     * @note This function acquires the I2C master mutex
     * @note Exits once the read operation is complete
    */
    status read(const i2c_address address, uint8_t* buffer, const size_t size);

    /**
     * Puts the specified address on the bus (in write mode) to check for ACK signal
     * @note This function acquires the I2C master mutex
    */
    status probe(const i2c_address address);

    /**
     * Resets the I2C master object state and releases the bus if it is held
     * @note Constructors should use this function to initialize the device
    */
    status reset();

    /** @todo Add async write/read using dma, and add registrable callbacks with status as parameter */


private:
    rtos::mutex mutex;

    virtual status write_handler(const i2c_address address, const uint8_t* data, const size_t size) = 0;
    virtual status read_handler(const i2c_address address, uint8_t* buffer, const size_t size) = 0;
    virtual status probe_handler(const i2c_address address) = 0;
    virtual status reset_handler() = 0;

};

}