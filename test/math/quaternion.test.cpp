#include "emblib/math/quaternion.hpp"
#include "emblib/math/vector.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Quaternion vector rotate", "[math][quaternion]")
{
    using emblib::math::vector3f;
    using emblib::math::quaternionf;

    vector3f v {1, 0, -1};
    quaternionf q1 {1, 0, 0, 0};
    quaternionf q2 {0, 1, 0, 0};
    quaternionf q3 {0, 0, 1, 0};
    quaternionf q4 {0, 0, 0, 1};

    REQUIRE((q1.rotate_vec(v) == v).all());
    REQUIRE((q2.rotate_vec(v) == vector3f{1, 0, 1}).all());
    REQUIRE((q3.rotate_vec(v) == vector3f{-1, 0, 1}).all());
    REQUIRE((q4.rotate_vec(v) == vector3f{-1, 0, -1}).all());
}