#include "emblib/dsp/iir.hpp"
#include "emblib/math/vector.hpp"
#include "catch2/catch_test_macros.hpp"
#include <vector>

TEST_CASE("IIR TF2", "[dsp][iir]")
{
    using emblib::dsp::iir_tf2;

    iir_tf2<float, 3> filter({1, 2, 3}, {3, 5, 7});
    std::vector<float> output;

    for (float x : {1, 2, 3, 4}) {
        filter.update(x);
        output.push_back(filter.get_output());
    }

    REQUIRE(output == std::vector<float> {1, 1, 2, -2});
}

TEST_CASE("IIR TF2 Vectorized", "[dsp][iir]")
{
    using emblib::dsp::iir_tf2;
    using emblib::math::vector3f;

    iir_tf2<vector3f, 3, float> filter({1, 2, 3}, {3, 5, 7});
    std::vector<vector3f> input {{1, 7, 3}, {2, 3, 4}, {5, 3, 9}};

    for (const auto& x : input) {
        filter.update(x);
    }
    auto output = filter.get_output();
    vector3f expected {4, 7, 8};
    REQUIRE((output == expected).all());
}