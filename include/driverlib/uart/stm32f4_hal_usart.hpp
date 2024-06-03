#pragma once

#include "emblib/emblib.hpp"
#include "emblib/drivers/uart.hpp"
#include "stm32f4xx_hal.h"

namespace emblib::drivers {

class stm32f4_hal_usart : public uart {

public:
    explicit stm32f4_hal_usart(const USART_HandleTypeDef& handle)
        : usart_handle(handle) {}

private:
    const uint32_t TIMEOUT = 0;

    USART_HandleTypeDef usart_handle;

    status write_handler(const uint8_t* data, const size_t size)
    {
        HAL_StatusTypeDef ret_status = HAL_USART_Transmit(&usart_handle, const_cast<uint8_t*>(data), size, TIMEOUT);

        return ret_status == HAL_OK ? status::OK : status::ERROR;
    }

    status read_handler(uint8_t* buffer, const size_t size)
    {
        HAL_StatusTypeDef ret_status = HAL_USART_Receive(&usart_handle, buffer, size, TIMEOUT);

        return ret_status == HAL_OK ? status::OK : status::ERROR;
    }

    status probe_handler()
    {
        return status::ERROR_NOTIMPL;
    }


};

}