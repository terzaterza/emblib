#pragma once

#include "emblib.hpp"
#include "drivers/i2c_address.hpp"
#include "drivers/i2c_master.hpp"
#include "stm32f4xx_hal.h"

namespace emblib::drivers {

class stm32f4_hal_i2c_master : public i2c_master {

public:
    explicit stm32f4_hal_i2c_master(I2C_HandleTypeDef& i2c_handle)
        : i2c_handle(i2c_handle) {}

private:
    const uint32_t TIMEOUT = 0;
    const uint32_t PROBE_TRIALS = 1;

    I2C_HandleTypeDef& i2c_handle;
    

    status write_handler(const i2c_address address, const uint8_t* data, const size_t size) override
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Master_Transmit(&i2c_handle, address << 1, const_cast<uint8_t*>(data), size, TIMEOUT);
        
        return hal_status == HAL_OK ? status::OK : status::ERROR;
    }
    
    status read_handler(const i2c_address address, uint8_t* buffer, const size_t size) override
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_Master_Receive(&i2c_handle, address << 1, buffer, size, TIMEOUT);
        
        return hal_status == HAL_OK ? status::OK : status::ERROR;
    }
    
    status probe_handler(const i2c_address address) override
    {
        HAL_StatusTypeDef hal_status = HAL_I2C_IsDeviceReady(&i2c_handle, address << 1, PROBE_TRIALS, TIMEOUT);
        
        return hal_status == HAL_OK ? status::OK : status::ERROR;
    }
    
    status reset_handler() override
    {
        return status::ERROR_NOTIMPL;
    }
    
};

}