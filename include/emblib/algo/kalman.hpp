#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/matrix.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::algo {

template <size_t state_dim>
class kalman_filter {

public:
    using state_vec = math::vector<float, state_dim>;
    using state_mat = math::matrix<float, state_dim, state_dim>;

public:
    explicit kalman_filter(
        const state_mat& state_transition_matrix,
        const state_mat& state_covariance_matrix
    ) : F(state_transition_matrix), Q(state_covariance_matrix)
    {}

    /**
     * Update the kalman filter state
     * @param H state to observation mapping matrix
     * @param R observation (measurment) covariance matrix
     * @param B input to state conversion matrix
     */
    template <size_t obs_dim, size_t input_dim>
    void update(
        const math::vector<float, obs_dim>& observation,
        const math::matrix<float, state_dim, obs_dim>& H,
        const math::matrix<float, obs_dim, obs_dim>& R,
        const math::vector<float, input_dim>& input,
        const math::matrix<float, state_dim, input_dim>& B
    );

private:
    /**
     * Current filter state
     */
    state_vec state;

    /**
     * State estimate covariance matrix
     */
    state_mat P;

    /**
     * State transition matrix
     */
    const state_mat F;

    /**
     * Process covariance matrix
     * @note const currently, can also be changed to a reference
     * and modified externally based on the process state
     */
    const state_mat Q;
};


template <size_t state_dim>
template <size_t obs_dim, size_t input_dim>
inline void kalman_filter<state_dim>::update(
    const math::vector<float, obs_dim> &observation,
    const math::matrix<float, state_dim, obs_dim>& H,
    const math::matrix<float, obs_dim, obs_dim>& R,
    const math::vector<float, input_dim> &input,
    const math::matrix<float, state_dim, input_dim>& B)
{
    using obs_vec = math::vector<float, obs_dim>;
    using obs_mat = math::matrix<float, obs_dim, obs_dim>;

    state_vec state_predict = F * state + B * input;
    state_mat cov_predict = F * P * F.transpose() + Q;

    obs_vec obs_diff = observation - H * state;
    obs_mat obs_cov = H * P * H.transpose() + R;

    math::matrix<float, state_dim, obs_dim> kalman_gain =
        math::matrix_divide_right(cov_predict * H.transpose(), obs_cov);

    state = state_predict + kalman_gain * obs_diff;
    P = cov_predict - kalman_gain * H * cov_predict;
}

}