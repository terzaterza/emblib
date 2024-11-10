#pragma once

#include "emblib/driver/io/char_dev.hpp"
#include "emblib/rtos/mutex.hpp"

#include "etl/string.h"
#include "etl/to_string.h"

namespace emblib {

enum class log_level_e {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

template <size_t BUFFER_SIZE>
class logger {

public:
    explicit logger(char_dev& log_device) :
        m_log_device(log_device)
    {}

    template <typename... item_types>
    void log(log_level_e level, item_types... items) noexcept
    {
        if (!m_log_device || level < m_output_level) {
            return;
        }

        m_mutex.lock();
        
        (log_item(items), ...);

        /** @todo Add writing the level */

        m_log_device->write(m_buffer.c_str(), m_buffer.size());
        m_buffer.clear();
        
        /** @todo Instead of clear can remove number of written
         * bytes from the beginning of the buffer */

        m_mutex.unlock();
    }

    void set_output_level(log_level_e level) noexcept
    {
        m_output_level = level;
    }

private:
    void log_item(const char* msg) noexcept
    {
        m_buffer.append(msg);
    }

    template <size_t N>
    void log_item(const char (&msg)[N]) noexcept
    {
        m_buffer.append(msg);
    }

    template <typename num_type, typename = typename std::enable_if<std::is_arithmetic<num_type>::value, num_type>::type>
    void log_item(num_type number) noexcept
    {
        etl::to_string(number, m_buffer, true);
    }

private:
    log_level_e m_output_level = log_level_e::INFO;
    etl::string<BUFFER_SIZE> m_buffer;
    char_dev& m_log_device;
    mutex m_mutex;
};

}