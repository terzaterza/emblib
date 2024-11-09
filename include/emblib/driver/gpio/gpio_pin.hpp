#pragma once

#include "emblib/emblib.hpp"

namespace emblib::driver {

class gpio_pin {

public:
    explicit gpio_pin() = default;
    virtual ~gpio_pin() = default;

    /* Copy operations not allowed */
    gpio_pin(const gpio_pin&) = delete;
    gpio_pin& operator=(const gpio_pin&) = delete;

    /* Move operations not allowed */
    gpio_pin(gpio_pin&&) = delete;
    gpio_pin& operator=(gpio_pin&&) = delete;

    /** @todo Can change to enum if needed */
    typedef bool pin_state_t;

    /**
     * Read the current state of the pin into the user provided buffer
     * @returns `true` if the `state` value now has valid data
     */
    virtual bool read(pin_state_t& state) noexcept = 0;

    /**
     * Write the given state to the pin
     * @returns `true` for a successful write
     */
    virtual bool write(pin_state_t state) noexcept = 0;
    
    /**
     * Toggle the current state
     * @returns `true` if toggled successfully
     */
    virtual bool toggle() noexcept = 0;

    /** @todo Add set_callback, set_interrupt_mode, set_pull_mode */

};

}

#if EMBLIB_UNNEST_NAMESPACES
namespace emblib {
    using namespace driver;
}
#endif