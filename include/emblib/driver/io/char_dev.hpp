#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/time.hpp"
#include <functional>

#if EMBLIB_CHAR_DEV_SUPPORT_ETL
#include "etl/string.h"
#endif

namespace emblib::driver {

/**
 * Base class for all objects which want to provide an interface
 * for writing and reading character (byte) streams
 */
class char_dev {

public:
    /* Typedef of callback functions for async operations */
    using callback_t = std::function<void(ssize_t)>;

    explicit char_dev() = default;
    virtual ~char_dev() = default;

    /* Copy operations not allowed */
    char_dev(const char_dev&) = delete;
    char_dev& operator=(const char_dev&) = delete;

    /* Move operations not allowed */
    char_dev(char_dev&&) = delete;
    char_dev& operator=(char_dev&&) = delete;

    /**
     * Write an array of bytes to the device
     * 
     * If `timeout` is 0, the function exits as soon as it tried
     * to write any data to the device. It's possible that all of the
     * data was written on the first try, but it's also possible
     * that none of the data was written. Timeout value of
     * `milliseconds::max()` represents waiting infinitely for all
     * of the data to be written.
     * 
     * @return If a negative value was returned, there was an error
     * during the write operation and it can't be assumed if any data
     * was written. A non-negative return value indicates that there
     * was no error and represents the number of bytes successfully
     * written before the timeout period passed.
    */
    virtual ssize_t write(const char* data, size_t size, milliseconds timeout = milliseconds::max()) noexcept = 0;

    /**
     * Read up to `size` bytes into the buffer
     * 
     * If `timeout` is 0, the function exits as soon as it tried
     * to read any data from the device. If no data was available, the
     * return value should be 0, else the number of bytes that was
     * available and read is returned. A positive value for the timeout
     * means that the function should wait up to `timeout` until at least
     * 1 byte of data is available and then read all the data available
     * and return. This doesn't mean that all `size` bytes will be read.
     * To wait until exactly `size` bytes are read, refer to `readall`.
     * 
     * @return If a negative value was returned, there was an error
     * during the read operation and it can't be assumed if any data
     * was read. A non-negative return value indicates that there
     * was no error and represents the number of bytes successfully read.
    */
    virtual ssize_t read(char* buffer, size_t size, milliseconds timeout = milliseconds::max()) noexcept = 0;

    /**
     * Start an async write
     * @return `true` if write operation started successfully
     */
    virtual bool write_async(const char* data, size_t size, const callback_t cb = callback_t()) noexcept
    {
        return false;
    }

    /**
     * Start an async read
     * @return `true` if write operation started successfully
     */
    virtual bool read_async(char* buffer, size_t size, const callback_t cb = callback_t()) noexcept
    {
        return false;
    }

    /**
     * Tests if the device is responding
     * @returns `true` if device responds
     * @note Default implementation is a dummy read
    */
    virtual bool probe(milliseconds timeout) noexcept
    {
        return read(nullptr, 0, timeout) == 0;
    }

    /**
     * @return `true` if this char device supports async operations
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

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif