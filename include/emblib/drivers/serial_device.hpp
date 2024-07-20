#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/rtos/mutex.hpp"

#include "etl/string.h"

namespace emblib::drivers {

class serial_device {

/* Class private typedef for callback functions */
template <typename context_t>
using callback_t = void (*)(status, context_t*);

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
     * Write overload for string objects
     */
    template <size_t size>
    status write(const etl::string<size>& string) noexcept
    {
        return write(string.c_str(), string.size());
    }

    /**
     * Write async overload for string objects
     */
    template <size_t size>
    status write_async(const etl::string<size>& string) noexcept
    {
        return write_async(string.c_str(), string.size());
    }

    /**
     * @return `true` if current serial device supports async operations
     */
    virtual bool async_available() noexcept
    {
        return false;
    }

    /**
     * Set the async write callback
     */
    template <typename context_t>
    void set_write_callback(callback_t<context_t> callback, context_t* ctx) noexcept
    {
        write_callback = reinterpret_cast<callback_t<void>>(callback); /** @todo Switch to using static cast */
        write_callback_context = ctx; /** @todo Can pass context as param when calling async_write */
    }

    /**
     * Set the async read callback
     */
    template <typename context_t>
    void set_read_callback(callback_t<context_t> callback, context_t* ctx) noexcept
    {
        read_callback = reinterpret_cast<callback_t<void>>(callback);
        read_callback_context = ctx; /** @todo Can pass context as param when calling async_read */
    }

protected:
    /**
     * To be called by serial device implementation on finishing async write
     * @note Use `friend` to give external functions access to this
     */
    void write_async_complete(status ret_status) noexcept
    {
        assert(mutex.unlock() == status::OK);
        if (write_callback) {
            write_callback(ret_status, write_callback_context);
        }
    }

    /**
     * To be called by serial device implementation on finishing async read
     * @note Use `friend` to give external functions access to this
     */
    void read_async_complete(status ret_status) noexcept
    {
        assert(mutex.unlock() == status::OK);
        if (read_callback) {
            read_callback(ret_status, read_callback_context);
        }
    }

private:
    /**
     * Implementation of write operation
     */
    virtual status write_handler(const char* data, size_t size) = 0;

    /**
     * Implementation of read operation
     */
    virtual status read_handler(char* buffer, size_t size) = 0;

    /**
     * Async operations not supported by default
     */
    virtual status write_async_handler(const char* data, size_t size)
    {
        UNUSED(data);
        UNUSED(size);
        return status::ERROR_NOTIMPL;
    }

    /**
     * Async operations not supported by default
     */
    virtual status read_async_handler(char* buffer, size_t size)
    {
        UNUSED(buffer);
        UNUSED(size);
        return status::ERROR_NOTIMPL;
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

    callback_t<void> write_callback = nullptr;
    callback_t<void> read_callback = nullptr;
    void* write_callback_context = nullptr;
    void* read_callback_context = nullptr;
};

}