#pragma once

#include "emblib/emblib.hpp"
#include "emblib/rtos/mutex.hpp"

#include "etl/string.h"

namespace emblib::drivers {

class serial_device {

/* Class private typedef for callback functions */
template <typename context_t>
using callback_t = void (*)(ssize_t, context_t*);

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
     * @returns `-1` if error, else number of bytes written
     * @note This function acquires the serial device mutex
     * @note Exits once the write operation is complete
    */
    ssize_t write(const char* data, size_t size) noexcept;

    /**
     * Read up to `size` bytes into the buffer
     * @returns `-1` if error, else number of bytes read
     * @note This function acquires the serial device mutex
     * @note Exits once the read operation is complete
    */
    ssize_t read(char* buffer, size_t size) noexcept;

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     * @note Internal mutex is locked until the write is started (released before this function returns)
     */
    bool write_async(const char* data, size_t size) noexcept;

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     * @note Internal mutex is acquired before the write start, and is
     * given to the user (if start successful) for unlocking once the operation finishes
     * @todo Instead of giving a pointer to the mutex, can just give
     * pointer to a function which releases it (bool (serial_device::*)() noexcept)
     */
    bool write_async(const char* data, size_t size, rtos::mutex*& mutex) noexcept;

    /**
     * Start an async read
     * @return `true` if write operation started successfully
     * @note Internal mutex is locked until the write is started (released before this function returns)
     */
    bool read_async(char* buffer, size_t size) noexcept;
    
    /**
     * Start an async read
     * @note Internal mutex is acquired before the read start, and is
     * given to the user (if start successful) for unlocking once the operation finishes
     * @return `true` if read operation started successfully
     */
    bool read_async(char* buffer, size_t size, rtos::mutex*& mutex) noexcept;

    /**
     * Tests if the device is responding
     * @returns `true` if device responds
     * @note Can be implemented as a dummy read
    */
    bool probe() noexcept;


    /**
     * Write overload for string objects
     */
    template <size_t size>
    ssize_t write(const etl::string<size>& string) noexcept
    {
        return write(string.c_str(), string.size());
    }

    /**
     * Write async overload for string objects
     */
    template <size_t size>
    bool write_async(const etl::string<size>& string) noexcept
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
     * @note Set `bytes_written` to -1 on error
     * @note Use `friend` to give external functions access to this
     */
    void write_async_complete(ssize_t bytes_written) noexcept
    {
        /* Can't unlock mutex here because this is called from ISR context */
        // assert(mutex.unlock() == status::OK);
        if (write_callback) {
            write_callback(bytes_written, write_callback_context);
        }
    }

    /**
     * To be called by serial device implementation on finishing async read
     * @note Set `bytes_read` to -1 on error
     * @note Use `friend` to give external functions access to this
     */
    void read_async_complete(ssize_t bytes_read) noexcept
    {
        /* Can't unlock mutex here because this is called from ISR context */
        // assert(mutex.unlock() == status::OK);
        if (read_callback) {
            read_callback(bytes_read, read_callback_context);
        }
    }

private:
    /**
     * Implementation of write operation
     */
    virtual ssize_t write_handler(const char* data, size_t size) noexcept = 0;

    /**
     * Implementation of read operation
     */
    virtual ssize_t read_handler(char* buffer, size_t size) noexcept = 0;

    /**
     * Async operations not supported by default
     */
    virtual bool write_async_handler(const char* data, size_t size) noexcept
    {
        UNUSED(data);
        UNUSED(size);
        return false;
    }

    /**
     * Async operations not supported by default
     */
    virtual bool read_async_handler(char* buffer, size_t size) noexcept
    {
        UNUSED(buffer);
        UNUSED(size);
        return false;
    }

    /**
     * Default implementation of probing as a dummy read
     */
    virtual bool probe_handler() noexcept
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