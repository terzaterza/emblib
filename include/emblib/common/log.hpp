#pragma once

#include "emblib/emblib.hpp"
#include "emblib/drivers/serial_device.hpp"

#include "etl/string.h"
#include "etl/to_string.h"

namespace emblib::log {

enum class log_level {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR
};

class logger {

public:
    /**
     * Default constructor
     */
    explicit logger(drivers::serial_device* log_device = nullptr, bool async = false) noexcept
        : log_device(log_device), async(async) {}

    /**
     * Set the log level for the following messages
     * @note If there is data in the buffer, this will flush it
    */
    logger& operator<<(log_level level);
    
    /**
     * Write generic data to the logger
     * @note Log flush will only be triggered if data ends with `\\n`
    */
    template <typename T>
    logger& operator<<(const T& data)
    {
        etl::to_string(data, buffer, true);

        if (buffer.size() == buffer.max_size() || buffer.back() == '\n') {
            flush();
        }
        return *this;
    }

    /**
     * Write const char array to the logger
     * @note Log flush will only be triggered if data ends with `\\n`
    */
    template <size_t n>
    logger& operator<<(const char (&data)[n])
    {
        buffer.append(data, n-1);

        if (buffer.size() == buffer.max_size() || buffer.back() == '\n') {
            flush();
        }
        return *this;
    }

    /**
     * Writes the current buffer to the serial devices and clears the buffer
     * 
     * @note Log buffer will be flushed if the character array ends
     * with `\\n` or if this method is explicitly called
    */
    void flush() noexcept;

    /**
     * Set the global logging level
    */
    static void set_global_level(log_level level) noexcept
    {
        logger::global_output_level = level;
    }

    /**
     * Set output serial device
     * @note Flushes the buffer if not it's empty and an output device already exists
     */
    void set_output_device(drivers::serial_device& device) noexcept;

private:
    /**
     * Determines which messages will be outputed during runtime
    */
    static log_level global_output_level;

    /**
     * All messages with `msg_level` >= `logger::global_output_level` will be output during runtime
     * @note Can only be set via << operator
    */
    log_level msg_level = log_level::INFO;
    
    /**
     * Serial device for writing the log data
    */
    drivers::serial_device* log_device;

    /**
     * Data buffer
    */
    etl::string<LOGGER_BUFFER_SIZE> buffer;

    /**
     * Write to log device in async mode if available
     */
    bool async;

};

}