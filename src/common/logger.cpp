#include "emblib/common/logger.hpp"

#include "etl/to_string.h"

namespace emblib {

void logger::flush() noexcept
{
    static const std::array<etl::string<5>, 4> prefix = { "DEBUG", "INFO", "WARN", "ERROR" };
    
    if (!m_log_device || m_msg_level < m_output_level) {
        return;
    }

    etl::string<LOGGER_BUFFER_SIZE + 8> output_str = prefix[static_cast<int>(m_msg_level)];
    output_str += ": ";
    output_str += m_buffer;
    m_buffer.clear();
    
    m_log_device->write(output_str.c_str(), output_str.size());
}

}