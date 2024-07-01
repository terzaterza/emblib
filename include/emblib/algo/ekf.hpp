#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/matrix.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::algo {

template <size_t state_dim, size_t obs_dim>
class ekf {

public:
using state_vec = math::vector<float, state_dim>;
using state_mat = math::matrix<float, state_dim, state_dim>;
using obs_vec   = math::vector<float, obs_dim>;
using obs_mat   = math::matrix<float, obs_dim, state_dim>;

using state_transition = state_vec (*)(const state_vec& state);
using obs_transition   = obs_vec (*)(const state_vec& state);
using state_jacob = state_mat (*)(const state_vec& state);
using obs_jacob   = obs_mat (*)(const state_vec& state);

public:
    explicit ekf(
        state_transition state_transition_func,
        obs_transition obs_transition_func,
        state_jacob state_jacob_func,
        obs_jacob obs_jacob_func,
        math::matrix<float, state_dim, state_dim> process_covariance,
        math::matrix<float, obs_dim, obs_dim> obs_covariance
    ) noexcept {}

    /**
     * Update the state of the filter with a new observation
     * 
     * @todo Add external input vector to f and update
    */
    void update(const obs_vec& observation) noexcept
    {
        state_mat Fk = F(state);

        state_vec state_predict = f(state);
        state_mat cov_predict = Fk * cov * math::transpose(Fk) + Q;

        obs_vec obs_diff = observation - h(state_predict);
        obs_mat Hk = H(state_predict);
        obs_mat HkT = math::transpose(Hk);

        auto Pk_HkT = cov_predict * HkT;
        auto residual_cov = Hk * Pk_HkT + R;
        auto kalman_gain = Pk_HkT * math::inverse(residual_cov);

        state = state_predict + kalman_gain * obs_diff;
        cov = (math::identity<float, state_dim>() - kalman_gain * Hk) * cov_predict;
    }

    /**
     * Current state of the kalman filter
     * 
     * @returns Copy of the state vector
    */
    state_vec get_state() noexcept
    {
        return state;
    }

private:
    state_vec state;
    state_mat cov;

    const state_transition f;
    const obs_transition h;
    const state_jacob F;
    const obs_jacob H;
    const math::matrix<float, state_dim, state_dim> Q;
    const math::matrix<float, obs_dim, obs_dim> R;


};

}