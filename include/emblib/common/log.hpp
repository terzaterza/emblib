#pragma once

#include "emblib/emblib.hpp"
#include "emblib/drivers/serial_device.hpp"
#include "emblib/math/matrix.hpp"
#include "emblib/math/vector.hpp"

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
        : log_device(log_device), async(async)
    {
        format.precision(2);
    }

    /**
     * Set the log level for the following messages
     * @note If there is data in the buffer, this will flush it
    */
    logger& operator<<(log_level level);

    /**
     * Write const char array to the logger
     * @note Log flush will only be triggered if data ends with `\\n`
    */
    template <size_t n>
    logger& operator<<(const char (&data)[n]);

    /**
     * Write a matrix to the logger
     * @note Flushes the buffer
     * @todo Move to matrix.hpp as external overload
    */
    template <typename scalar_t, size_t rows, size_t cols>
    logger& operator<<(const math::matrix<scalar_t, rows, cols>& matrix);

    /**
     * Write a vector to the logger
     * @note Flushes the buffer
     * @todo Move to vector.hpp as external overload
    */
    template <typename scalar_t, size_t dim>
    logger& operator<<(const math::vector<scalar_t, dim>& vector);

    /* Operator overloads */
    logger& operator<<(int n)           { write(n); return *this; }
    logger& operator<<(unsigned n)      { write(n); return *this; }
    logger& operator<<(long n)          { write(n); return *this; }
    logger& operator<<(unsigned long n) { write(n); return *this; }
    logger& operator<<(float n)         { write(n); return *this; }
    logger& operator<<(double n)        { write(n); return *this; }

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
     * Write numeric data to the logger
     * @note Doesn't trigger a flush
     * @warning Argument is copied - use only for numbers
     * @todo Add enable if to template and make public overload
    */
    template <typename T>
    void write(const T data);

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
     * String format
     */
    etl::format_spec format;

    /**
     * Write to log device in async mode if available
     */
    bool async;

};


template <typename T>
inline void logger::write(const T data)
{
    etl::to_string(data, buffer, format, true);

    if (buffer.size() == buffer.max_size() || buffer.back() == '\n') {
        flush();
    }
}

template <size_t n>
inline logger &logger::operator<<(const char (&data)[n])
{
    buffer.append(data, n-1);

    if (buffer.size() == buffer.max_size() || buffer.back() == '\n') {
        flush();
    }
    return *this;
}

template <typename scalar_t, size_t rows, size_t cols>
inline logger &logger::operator<<(const math::matrix<scalar_t, rows, cols> &matrix)
{
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            etl::to_string(matrix(r, c), buffer, format, true);
            buffer.append(" ", 1);
        }
        buffer.append("\n", 1);
    }
    flush();
    return *this;
}

template <typename scalar_t, size_t dim>
inline logger &logger::operator<<(const math::vector<scalar_t, dim> &vector)
{
    for (size_t i = 0; i < dim; i++) {
        etl::to_string(vector(i), buffer, format, true);
        buffer.append(" ", 1);
    }
    buffer.append("\n", 1);
    flush();
    return *this;
}

}