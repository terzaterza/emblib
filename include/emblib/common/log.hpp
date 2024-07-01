#pragma once

#include "emblib/emblib.hpp"
#include "emblib/drivers/serial_device.hpp"

#include <string>
#include <iostream>

namespace emblib::log {

enum class log_level {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR
};

class logger {

public:

    explicit logger(drivers::serial_device* log_device = nullptr) noexcept
        : log_device(log_device), msg_level(log_level::INFO) {}

    /**
     * Set the log level for the following messages
     * @note If there is data in the buffer, this will flush it
    */
    logger& operator<<(log_level level) noexcept
    {
        if (this->buffer.size() > 0)
            this->flush();

        this->msg_level = level;
    }
    
    /**
     * Write generic data to the logger
     * @note Log flush will only be triggered if data ends with `\\n`
    */
    template <typename T>
    logger& operator<<(const T& data) noexcept
    {
        std::string temp = std::to_string(data);
        buffer += temp;

        if (buffer.back() == '\n')
            flush();
    }

    /**
     * Writes the current buffer to the serial devices and clears the buffer
     * 
     * @note Log buffer will be flushed if the character array ends
     * with `\\n` or if this method is explicitly called
    */
    void flush() noexcept
    {
        if (!log_device || msg_level < logger::global_output_level)
            return;

        static const std::string prefix[] = { "DEBUG", "INFO", "WARN", "ERROR" };

        const std::string prefix_str = prefix[(int)this->msg_level] + ": ";
        const std::string output_str = prefix_str + buffer;

        log_device->write((const uint8_t*)output_str.c_str(), output_str.size());
    }

    /**
     * Set the global logging level
    */
    void set_global_level(log_level level) noexcept
    {
        logger::global_output_level = level;
    }

private:
    /**
     * Determines which messages will be outputed during runtime
    */
    static log_level global_output_level;

    /**
     * All messages with `msg_level` >= `logger::global_output_level` will be output during runtime
     * @note Can only be set via << operator
    */
    log_level msg_level;
    
    /**
     * Serial device for writing the log data
    */
    drivers::serial_device* log_device;

    /**
     * Data buffer
    */
    std::string buffer;

};

}