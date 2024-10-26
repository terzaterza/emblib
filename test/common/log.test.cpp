#include "emblib/common/log.hpp"
#include <iostream>
#include "catch2/catch_test_macros.hpp"

using namespace emblib;

class cout_sdev : public emblib::drivers::serial_device {

private:
    ssize_t write_handler(const char* data, size_t size) noexcept override
    {
        try {
            std::cout.write(data, size);
        } catch(...) {
            return -1;
        }
        return size;
    }

    ssize_t read_handler(char* buffer, size_t size) noexcept override
    {
        try {
            std::cin.read(buffer, size);
        } catch(...) {
            return -1;
        }
        return size;
    }

};

TEST_CASE("Logger test", "[common][logger][log]")
{
    cout_sdev sdev;
    log::logger logger(&sdev);

    logger << log::log_level::INFO << "Info message!";
    logger << log::log_level::WARNING << "Warning message!\n";

    /** @todo Change sdev to store printed message and then compare to expected */
    REQUIRE(true);
}