#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/rtos/mutex.hpp"

namespace emblib::drivers {

class serial_device {

/* Class private typedef for callback functions */
using callback_t = void (*)(status);
using callback_priv_t = void (serial_device::*)(status);

public:
    explicit serial_device() = default;
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
    status write(const char* data, size_t size) noexcept;

    /**
     * Read an array of bytes
     * @note This function acquires the serial device mutex
     * @note Exits once the read operation is complete
    */
    status read(char* buffer, size_t size) noexcept;

    /**
     * Start an async write
     * @note Internal mutex is locked until the write is complete (released just before callback is called)
     * @return `true` if write operation started successfully
     */
    status write_async(const char* data, size_t size) noexcept;

    /**
     * Start an async read
     * @note Internal mutex is locked until the read is complete (released just before callback is called)
     * @return `true` if write operation started successfully
     */
    status read_async(char* buffer, size_t size) noexcept;

    /**
     * Tests if the device is responding
     * @note Can be implemented as a dummy read
    */
    status probe() noexcept;

    /**
     * @return `true` if current serial device supports async operations
     */
    virtual bool async_available() noexcept = 0;

    /**
     * Set the async write callback
     */
    void set_write_callback(callback_t callback) noexcept
    {
        write_callback = callback;
    }

    /**
     * Set the async read callback
     */
    void set_read_callback(callback_t callback) noexcept
    {
        read_callback = callback;
    }

private:
    /**
     * To be called by serial device implementation on finishing async write
     */
    void async_write_complete(status ret_status) noexcept
    {
        assert(mutex.unlock() == status::OK);
        if (write_callback) {
            write_callback(ret_status);
        }
    }

    /**
     * To be called by serial device implementation on finishing async read
     */
    void async_read_complete(status ret_status) noexcept
    {
        assert(mutex.unlock() == status::OK);
        if (read_callback) {
            read_callback(ret_status);
        }
    }

    /**
     * Default implementation of probing as a dummy read
     */
    virtual status probe_handler() noexcept
    {
        return read_handler(nullptr, 0);
    }

private:
    rtos::mutex mutex;

    callback_t write_callback = nullptr;
    callback_t read_callback = nullptr;

    virtual status write_handler(const char* data, size_t size) = 0;
    virtual status read_handler(char* buffer, size_t size) = 0;
    virtual status write_async_handler(const char* data, size_t size, callback_priv_t callback) = 0;
    virtual status read_async_handler(char* buffer, size_t size, callback_priv_t callback) = 0;
};

}