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
     * Logger buffer size before needing to flush
     */
    static constexpr size_t BUFFER_CAPACITY = 128;

    /**
     * Default constructor
     */
    explicit logger(drivers::serial_device* log_device = nullptr) noexcept
        : log_device(log_device) {}

    /**
     * Set the log level for the following messages
     * @note If there is data in the buffer, this will flush it
    */
    logger& operator<<(log_level level) noexcept
    {
        if (buffer.size() > 0) {
            flush();
        }

        msg_level = level;
        return *this;
    }
    
    /**
     * Write generic data to the logger
     * @note Log flush will only be triggered if data ends with `\\n`
    */
    template <typename T>
    logger& operator<<(const T& data) noexcept
    {
        etl::to_string(data, buffer, true);

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
    void flush() noexcept
    {
        if (!log_device || msg_level < logger::global_output_level) {
            return;
        }

        static const std::array<etl::string<5>, 4> prefix = { "DEBUG", "INFO", "WARN", "ERROR" };
        const etl::string<BUFFER_CAPACITY + 8> output_str = prefix[(int)this->msg_level] + ": " + buffer;

        log_device->write(reinterpret_cast<const uint8_t*>(output_str.c_str()), output_str.size());
    }

    /**
     * Set the global logging level
    */
    static void set_global_level(log_level level) noexcept
    {
        logger::global_output_level = level;
    }

    /**
     * Set output serial device
     */
    void set_output_device(drivers::serial_device& device) noexcept
    {
        this->log_device = &device;
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
    log_level msg_level = log_level::INFO;
    
    /**
     * Serial device for writing the log data
    */
    drivers::serial_device* log_device;

    /**
     * Data buffer
    */
    etl::string<buffer_size> buffer;

};

}