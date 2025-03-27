#include "emblib/driver/stdio_dev.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("std io test", "[driver][io]")
{
    emblib::stdio_dev dev;
    constexpr char test_str[] = "STDIO device test string\n";
    REQUIRE(dev.write(test_str, sizeof(test_str)) == sizeof(test_str));
}
