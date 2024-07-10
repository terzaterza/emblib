#pragma once

#include "emblib/emblib.hpp"
#include "emblib/math/matrix.hpp"
#include "emblib/math/vector.hpp"

namespace emblib::algo {

template <size_t state_dim, size_t obs_dim, typename ext_input_t = void>
class ekf {

public:
    using state_vec = math::vector<float, state_dim>;
    using obs_vec   = math::vector<float, obs_dim>;

    explicit ekf(
        const math::matrix<float, state_dim, state_dim>& process_covariance,
        const math::matrix<float, obs_dim, obs_dim>& obs_covariance
    ) noexcept : Q(process_covariance), R(obs_covariance) {}

    /**
     * Update the state of the filter with a new observation
    */
    state_vec update(const obs_vec& observation, const ext_input_t& ext_input) noexcept
    {
        state_jacob Fk = F(state, ext_input);

        state_vec state_predict = f(state, ext_input);
        state_jacob cov_predict = Fk * cov * math::transpose(Fk) + Q;

        obs_vec obs_diff = observation - h(state_predict);
        obs_jacob Hk = H(state_predict);
        obs_jacob HkT = math::transpose(Hk);

        auto Pk_HkT = cov_predict * HkT;
        auto residual_cov = Hk * Pk_HkT + R;
        auto kalman_gain = Pk_HkT * math::inverse(residual_cov);

        state = state_predict + kalman_gain * obs_diff;
        cov = (math::identity<float, state_dim>() - kalman_gain * Hk) * cov_predict;

        return state;
    }

    /**
     * Current state of the kalman filter
     * 
     * @returns Copy of the state vector
    */
    state_vec get_state() const noexcept
    {
        return state;
    }

protected:
    using state_jacob = math::matrix<float, state_dim, state_dim>;
    using obs_jacob   = math::matrix<float, obs_dim, state_dim>;

private:
    /**
     * State transition function
     * 
     * Compute the state at the next time step based on current state and external input
     */
    virtual state_vec f(const state_vec& state, const ext_input_t& ext_input) const noexcept = 0;

    /**
     * State Jacobian
     * 
     * Represents matrix of derivatives of each component of function `f`'s output
     * with respect to each component of the state vector
     */
    virtual state_jacob F(const state_vec& state, const ext_input_t& ext_input) const noexcept = 0;

    /**
     * Observation mapping function
     * 
     * Map the predicted state to the expected observation vector
     */
    virtual obs_vec h(const state_vec& state) const noexcept = 0;

    /**
     * Observation Jacobian
     * 
     * Matrix of derivatives of components of function `h`'s output
     * with respect to each component of the state vector
     */
    virtual obs_jacob H(const state_vec& state) const noexcept = 0;


    state_vec state;
    math::matrix<float, state_dim, state_dim> cov;

    const math::matrix<float, state_dim, state_dim> Q;
    const math::matrix<float, obs_dim, obs_dim> R;


};

}