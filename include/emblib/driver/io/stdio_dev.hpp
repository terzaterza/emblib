#pragma once

#include "emblib/emblib.hpp"
#include "emblib/driver/io/char_dev.hpp"
#include <iostream>

namespace emblib {

class stdio_dev : public emblib::char_dev {

public:
    ssize_t write(const char* data, size_t size, milliseconds timeout = MAX_MILLISECONDS) noexcept override
    {
        std::cout.write(data, size);
        // std::cout.flush();
        return size;
    }

    ssize_t read(char* buffer, size_t size, milliseconds timeout = MAX_MILLISECONDS) noexcept override
    {
        std::cin.read(buffer, size);
        return size;
    }
};

}