#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/char_dev.hpp"

#include "etl/string.h"
#include "etl/to_string.h"

namespace emblib {

enum class log_level_e {
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
    explicit logger(char_dev* log_device = nullptr, bool async = false) noexcept
        : m_log_device(log_device), m_msg_level(log_level_e::INFO) {}

    /**
     * Writes the current buffer to the output device and clears the buffer
     * 
     * @note Log buffer will be flushed if the character array ends
     * with `\\n` or if this method is explicitly called
    */
    void flush() noexcept;

    /**
     * Set output logging device
     * @note Flushes the buffer if not it's empty and an output device already exists
     */
    void set_output_device(char_dev& device) noexcept
    {
        if (!m_buffer.empty() && m_log_device != nullptr) {
            flush();
        }
        
        m_log_device = &device;
    }

    /**
     * Set the minimum logging level
    */
    void set_output_level(log_level_e level) noexcept
    {
        if (m_msg_level < level) {
            m_buffer.clear();
        }
        
        m_output_level = level;
    }

    /**
     * Set the log level for the following messages
     * @note If there is data in the buffer, this will flush it
    */
    logger& operator<<(log_level_e level)
    {
        if (!m_buffer.empty()) {
            flush();
        }
        m_msg_level = level;
        return *this;
    }
    
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
    template <size_t N>
    logger& operator<<(const char (&data)[N])
    {
        buffer.append(data, N-1);

        if (buffer.size() == buffer.max_size() || buffer.back() == '\n') {
            flush();
        }
        return *this;
    }

private:
    /**
     * Determines which messages will be outputed during runtime
    */
    log_level_e m_output_level;

    /**
     * All messages with msg level >= output level will be output during runtime
     * @note Can only be set via << operator
    */
    log_level_e m_msg_level;
    
    /**
     * Device for writing the log data
    */
    char_dev* m_log_device;

    /**
     * Data buffer
    */
    etl::string<LOGGER_BUFFER_SIZE> m_buffer;

};

/* Convinience functions */

static inline logger& logger_debug(logger& logger) noexcept
{
    return logger << log_level_e::DEBUG;
}

static inline logger& logger_info(logger& logger) noexcept
{
    return logger << log_level_e::INFO;
}

static inline logger& logger_warning(logger& logger) noexcept
{
    return logger << log_level_e::WARNING;
}

static inline logger& logger_error(logger& logger) noexcept
{
    return logger << log_level_e::ERROR;
}

}