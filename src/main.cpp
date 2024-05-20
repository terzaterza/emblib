#include "i2c/stm32f4_hal_i2c_master.hpp"

int main()
{
    I2C_HandleTypeDef h;
    emblib::drivers::stm32f4_hal_i2c_master stm32f4_i2c(h);
}