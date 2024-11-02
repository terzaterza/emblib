#include "emblib/rtos/mutex.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("RTOS mutex test", "[rtos][mutex]")
{
    emblib::rtos::mutex mutex;

    REQUIRE(mutex.lock());
    REQUIRE(mutex.unlock());
}