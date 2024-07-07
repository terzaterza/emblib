#include "emblib/algo/iir.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("IIR test", "[algo][iir]")
{
    emblib::algo::iir<int, 2, 1> iir({1, 2}, {1});

    iir.update(1);
    iir.update(3);
    
    REQUIRE(iir.get_output() == 5);
}