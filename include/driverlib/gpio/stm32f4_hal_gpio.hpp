#pragma once

#include "emblib/emblib.hpp"
#include "emblib/common/status.hpp"
#include "emblib/drivers/gpio.hpp"
#include "stm32f4xx_hal.h"

namespace emblib::drivers {

class stm32f4_hal_gpio : public gpio {

public:
    explicit stm32f4_hal_gpio(const GPIO_TypeDef& gpio_handle)
        : gpio_handle(gpio_handle) {}

    explicit stm32f4_hal_gpio(const gpio_config& config)
    {
        /** @todo Init GPIO typedef according to HAL user guide */
    }

    status read(gpio::state& state)
    {
        GPIO_PinState pin_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
    }

    status write(gpio::state state)
    {

    }

    status toggle()
    {

    }


private:
    GPIO_TypeDef gpio_handle;
    uint16_t pin_mask;

};

}