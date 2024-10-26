#include "emblib/algo/pid.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("PID test", "[algo][pid]")
{
    emblib::algo::pid<float> pid(1, 0.5, 0, 1);
}