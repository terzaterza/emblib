#include "emblib/algo/ekf.hpp"
#include "catch2/catch_test_macros.hpp"

using namespace emblib;

class ekf_test : public algo::ekf<6> {

public:
    ekf_test(state_mat& Q, state_vec& state) :
        ekf(Q, state)
    {}

private:
    using input_t = void*;

    state_vec f(const state_vec& state, const input_t& input) const noexcept override
    {
        return state.array() * state.array();
    }

    state_mat F(const state_vec& state, const input_t& input) const noexcept override
    {
        auto tmp = state * 2.f;
        return math::diagonal<float, 6>(
            {tmp(0), tmp(1), tmp(2), tmp(3), tmp(4), tmp(5)}
        );
    }
};

math::vec3f h(const math::vector<float, 6>& state) noexcept
{
    return math::subvector<0, 3>(state) * 2 + math::subvector<3, 6>(state) * 3;
}

auto H(const math::vector<float, 6>& state) noexcept
{
    static math::matrix<float, 3, 6> jacob;
    jacob <<    2, 0, 0, 3, 0, 0,
                0, 2, 0, 0, 3, 0,
                0, 0, 2, 0, 0, 3;
    return jacob;
}

TEST_CASE("EKF test", "[algo][ekf]")
{
    auto Q = math::diagonal<float, 6>({1, 0.5, 1, 0.5, 1, 0.5});
    auto R = math::diagonal<float, 3>({0.5, 1, 0.5});

    auto obs = math::vec3f(3, 1, 10);
    auto initial_state = math::vector<float, 6>({1, 0, -2, -1, 0.5, -1.5});

    ekf_test ekf(Q, initial_state);
    ekf.update<3>(obs, h, H, R);

    math::vector<float, 6> expected({0.555556, 0.020833, 2.944444, 0.666667, 0.312500, 1.458333});
    REQUIRE(ekf.get_state().isApprox(expected));
}