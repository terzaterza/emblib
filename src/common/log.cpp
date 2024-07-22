#include "emblib/common/log.hpp"

#include "etl/to_string.h"

namespace emblib::log {

log_level logger::global_output_level = log_level::INFO;


logger& logger::operator<<(log_level level)
{
    if (!buffer.empty()) {
        flush();
    }

    msg_level = level;
    return *this;
}


void logger::flush() noexcept
{
    static const std::array<etl::string<5>, 4> prefix = { "DEBUG", "INFO", "WARN", "ERROR" };
    
    if (!log_device || msg_level < logger::global_output_level) {
        return;
    }

    etl::string<LOGGER_BUFFER_SIZE + 8> output_str = prefix[static_cast<int>(this->msg_level)];
    output_str += ": ";
    output_str += buffer;
    buffer.clear();

    if (async && log_device->async_available()) {
        log_device->write_async(output_str);
    } else {
        log_device->write(output_str);
    }
}


void logger::set_output_device(drivers::serial_device& device) noexcept
{
    if (!buffer.empty()) {
        flush();
    }

    this->log_device = &device;
}

}