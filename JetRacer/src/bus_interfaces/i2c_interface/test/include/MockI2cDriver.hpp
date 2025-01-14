#include "II2cDriver.hpp"
#include <gmock/gmock.h>

class MockI2cDriver : public II2cDriver
{
    public:
        MockI2cDriver(uint8_t device) : II2cDriver(device){};
        MOCK_METHOD(int, open, (), (override));
        MOCK_METHOD(ssize_t, read, (std::vector<uint8_t> & buff), (override));
        MOCK_METHOD(ssize_t, write, (std::vector<uint8_t> & buff), (override));
        MOCK_METHOD(int, setAddress, (uint8_t address), (override));
};
