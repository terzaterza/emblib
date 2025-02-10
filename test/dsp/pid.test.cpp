#include "emblib/dsp/pid.hpp"
#include "emblib/math/vector.hpp"
#include "catch2/catch_test_macros.hpp"
#include <vector>

TEST_CASE("PID", "[dsp][pid]")
{
    using emblib::dsp::pid;

    pid<float> pid_(2, 1, 0, -5, 5);
    std::vector<float> output;

    for (float x : {1, 2, 3, 4}) {
        pid_.update(x);
        output.push_back(pid_.get_output());
    }
}

TEST_CASE("PID Vectorized", "[dsp][pid]")
{
    using emblib::dsp::pid;
    using emblib::math::vector3f;

    pid<vector3f, float> pid_(2, 1, 0.5, -5, 5);
    std::vector<vector3f> output;

    for (float x : {1, 2, 3, 4}) {
        pid_.update(x);
        output.push_back(pid_.get_output());
    }
}