#include "emblib/dsp/kalman.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Kalman linear update", "[dsp][kalman]")
{
    using emblib::dsp::kalman;
    using emblib::math::matrixf;
    using emblib::math::vectorf;
    
    kalman<3> kalman3({1, 1, 1});
    
    matrixf<3> F = {{1, 2, 3}, {-2, -4, 0}, {2, -1, 1}};
    vectorf<3> u = {1, 0, -1};
    matrixf<4, 3> H = {{1, 3, 7}, {4, 2, -1}, {-1, 2, 0}, {5, 0, -3}};

    vectorf<4> z = {2, -1, 3, 1};
    matrixf<3> Q = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // Identity
    matrixf<4> R = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}; // Identity
    
    kalman3.update<4>(F, u, H, Q, R, z);
    vectorf<3> state = kalman3.get_state();
    vectorf<3> expected = {0.348207, -0.381673, 0.407171};

    REQUIRE(state.get_base().isApprox(expected.get_base()));
}