#pragma once

#include "emblib.hpp"
#include "common/status.hpp"

namespace emblib::drivers {

struct gpio_config {
    gpio::mode mode;
    gpio::edge edge;
    gpio::pull pull;
};

class gpio {

public:
    enum class state {
        LOW, HIGH
    };

    enum class mode {
        INPUT, INPUT_IR, INPUT_ALT, OUTPUT_PP, OUTPUT_OD, OUTPUT_ALT
    };

    enum class pull {
        NONE, PULL_UP, PULL_DOWN
    };

    enum class edge {
        RISING, FALLING, BOTH
    };

public:
    explicit gpio() {}
    virtual ~gpio() = default;

    /* Copy operations not allowed */
    gpio(const gpio&) = delete;
    gpio& operator=(const gpio&) = delete;

    /* Move operations not allowed */
    gpio(gpio&&) = delete;
    gpio& operator=(gpio&&) = delete;

    /**
     * Read pin state
     * @note Return status can be an error if pin is not
     * in proper mode or communication with remote gpio fails
    */
    virtual status read(gpio::state& state) = 0;

    /**
     * Read pin state
     * @note Return status can be an error if pin is not
     * in proper mode or communication with remote gpio fails
    */
    virtual status write(gpio::state state) = 0;

    /**
     * Toggle pin state
     * @note Return status can be an error if pin is not
     * in proper mode or communication with remote gpio fails
    */
    virtual status toggle() = 0;

    /**
     * Set callback for GPIO interrupt
    */
    virtual void set_interrupt_callback(void (*callback)(void)) = 0;

    /** @todo Add methods for chaning mode, interrupt edge, ... */

};

}