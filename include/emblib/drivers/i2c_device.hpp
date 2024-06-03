#pragma once

#include "emblib/emblib.hpp"
#include "emblib/drivers/i2c_master.hpp"
#include "emblib/drivers/i2c_address.hpp"
#include "emblib/drivers/serial_device.hpp"

namespace emblib::drivers {

class i2c_device : public serial_device {

public:
    explicit i2c_device(i2c_master& i2c_bus, const i2c_address address)
        : i2c_bus(i2c_bus), address(address) {}

private:
    i2c_master& i2c_bus;
    const i2c_address address;

    status write_handler(const uint8_t* data, size_t size) override
    {
        return i2c_bus.write(this->address, data, size);
    }

    status read_handler(uint8_t* buffer, size_t size) override
    {
        return i2c_bus.read(this->address, buffer, size);
    }

    status probe_handler() override
    {
        return i2c_bus.probe(this->address);
    }

};

}