#pragma once

#include "emblib.hpp"
#include "common/status.hpp"
#include "rtos/mutex.hpp"

namespace emblib::drivers {

class serial_device {

public:
    serial_device() {}
    virtual ~serial_device() = default;

    /* Copy operations not allowed */
    serial_device(const serial_device&) = delete;
    serial_device& operator=(const serial_device&) = delete;

    /* Move operations not allowed */
    serial_device(serial_device&&) = delete;
    serial_device& operator=(serial_device&&) = delete;

    /**
     * Write an array of bytes
     * @note This function acquires the serial device mutex
     * @note Exits once the write operation is complete
    */
    status write(const uint8_t* data, const size_t size);

    /**
     * Read an array of bytes
     * @note This function acquires the serial device mutex
     * @note Exits once the read operation is complete
    */
    status read(uint8_t* buffer, const size_t size);

    /**
     * Tests if the device is responding
     * @note Can be implemented as a dummy read
    */
    status probe();

private:
    rtos::mutex mutex;

    virtual status write_handler(const uint8_t* data, const size_t size) = 0;
    virtual status read_handler(uint8_t* buffer, const size_t size) = 0;
    virtual status probe_handler() = 0;

};

}