#pragma once

#include "emblib/emblib.hpp"
#include <functional>

#if EMBLIB_CHAR_DEV_SUPPORT_ETL
#include "etl/string.h"
#endif

namespace emblib {

/**
 * Base class for all objects which want to provide an interface
 * for writing and reading character (byte) streams
 */
class char_dev {

/* Class private typedef of callback functions for async operations */
using callback_t = std::function<void(ssize_t)>;

public:
    explicit char_dev() = default;
    virtual ~char_dev() = default;

    /* Copy operations not allowed */
    char_dev(const char_dev&) = delete;
    char_dev& operator=(const char_dev&) = delete;

    /* Move operations not allowed */
    char_dev(char_dev&&) = delete;
    char_dev& operator=(char_dev&&) = delete;

    /**
     * Write an array of bytes
     * @returns `-1` if error, else number of bytes written
     * @note Exits once the write operation is complete
    */
    virtual ssize_t write(const char* data, size_t size) noexcept = 0;

    /**
     * Read up to `size` bytes into the buffer
     * @returns `-1` if error, else number of bytes read
     * @note Exits once the read operation is complete
    */
    virtual ssize_t read(char* buffer, size_t size) noexcept = 0;

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     */
    virtual bool write_async(const char* data, size_t size, const callback_t cb = callback_t()) noexcept = 0;

    /**
     * Start an async read
     * @return `true` if write operation started successfully
     */
    virtual bool read_async(char* buffer, size_t size, const callback_t cb = callback_t()) noexcept = 0;

    /**
     * Tests if the device is responding
     * @returns `true` if device responds
     * @note Default implementation is a dummy read
    */
    virtual bool probe() noexcept
    {
        return read(nullptr, 0) == 0;
    }

    /**
     * @return `true` if current serial device supports async operations
     */
    virtual bool is_async_available() noexcept
    {
        return false;
    }

#if EMBLIB_CHAR_DEV_SUPPORT_ETL
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
#endif

};

}