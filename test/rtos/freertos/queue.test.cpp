#include "emblib/rtos/freertos/queue.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("RTOS queue test", "[rtos][freertos][queue]")
{
    emblib::rtos::freertos::queue<int, 2> queue;
    int to_send = 2;
    int buffer = 0;

    queue.send(to_send, std::chrono::milliseconds(10));
    queue.receive(buffer, std::chrono::milliseconds(10));

    REQUIRE(to_send == buffer);
}