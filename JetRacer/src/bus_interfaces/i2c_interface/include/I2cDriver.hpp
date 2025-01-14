#pragma once

#include "II2cDriver.hpp"

const uint8_t MAX_RETRY = 5;

class I2cDriver : public II2cDriver
{
    public:
        I2cDriver(uint8_t device);
        ~I2cDriver();

        int open() override;
        ssize_t read(std::vector<uint8_t>& buff) override;
        ssize_t write(std::vector<uint8_t>& buff) override;
        int setAddress(uint8_t address) override;
};
