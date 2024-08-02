#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/matrix.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::algo {

template <size_t state_dim, typename input_t = void*>
class ekf {

public:
    using state_vec = math::vector<float, state_dim>;
    using state_mat = math::matrix<float, state_dim, state_dim>;
    using state_f   = state_vec (const state_vec&); /* State transition function type */
    using state_j   = state_mat (const state_vec&); /* State jacobian function type */

public:
    explicit ekf(
        const state_mat& state_covariance_matrix
    ) noexcept : Q(state_covariance_matrix)
    {}

    explicit ekf(
        const state_mat& state_covariance_matrix,
        const state_vec& initial_state
    ) noexcept : Q(state_covariance_matrix), state(initial_state)
    {}

    /**
     * Update the kalman filter state with a new observation
     * @param h state to observation mapping function
     * @param H state to observation jacobian (derivative of `h` with respect to the state)
     * @param R observation covariance matrix
     * @todo Can change `*h` and `*H` to `ekf<state_dim>::*h`, ... if access to internals needed
    */
    template <size_t obs_dim>
    void update(
        const math::vector<float, obs_dim>& observation,
        math::vector<float, obs_dim> (*h)(const state_vec& state),
        math::matrix<float, obs_dim, state_dim> (*H)(const state_vec& state),
        const math::matrix<float, obs_dim, obs_dim>& R,
        const input_t& input = nullptr
    );

    /**
     * Current state of the kalman filter
     * @returns Const reference to the state vector
    */
    const state_vec& get_state() const noexcept
    {
        return state;
    }

private:
    /**
     * State transition function
     * 
     * Compute the state at the next time step based on current state and external input
     */
    virtual state_vec f(const state_vec& state, const input_t& input) const noexcept = 0;

    /**
     * State Jacobian
     * 
     * Represents matrix of derivatives of each component of function `f`'s output
     * with respect to each component of the state vector
     */
    virtual state_mat F(const state_vec& state, const input_t& input) const noexcept = 0;

private:
    /**
     * Current filter state
     */
    state_vec state = state_vec::Zero();

    /**
     * State estimate covariance matrix
     */
    state_mat P = state_mat::Zero();

    /**
     * Process covariance matrix
     * @note const currently, can also be changed to a reference
     * and modified externally based on the process state
     */
    const state_mat Q;

};


template <size_t state_dim, typename input_t>
template <size_t obs_dim>
inline void ekf<state_dim, input_t>::update(
    const math::vector<float, obs_dim>& observation,
    math::vector<float, obs_dim> (*h)(const state_vec& state),
    math::matrix<float, obs_dim, state_dim> (*H)(const state_vec& state),
    const math::matrix<float, obs_dim, obs_dim>& R,
    const input_t& input
)
{
    using obs_vec = math::vector<float, obs_dim>;
    using obs_mat = math::matrix<float, obs_dim, obs_dim>;

    const state_vec state_predict = f(state, input);
    const state_mat Fj = F(state, input); /* State jacobian */
    const state_mat cov_predict = Fj * P * Fj.transpose() + Q;

    const math::matrix<float, obs_dim, state_dim> Hj = H(state_predict); /* Observation jacobian */
    const auto HjT = Hj.transpose();
    const obs_vec obs_diff = observation - h(state_predict);
    const obs_mat obs_cov = Hj * cov_predict * HjT + R;

    const math::matrix<float, state_dim, obs_dim> kalman_gain =
        math::matrix_divide_right<float, state_dim, obs_dim>(cov_predict * HjT, obs_cov);

    state = state_predict + kalman_gain * obs_diff;
    P = cov_predict - kalman_gain * Hj * cov_predict;
}

}