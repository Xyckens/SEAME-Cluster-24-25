#pragma once

#include <rclcpp/rclcpp.hpp>

/**
 * @class APCA9685Driver
 * @brief Abstract interface defining interactions with the PCA9685.
 *
 * This abstract class provides a hardware abstraction layer for pca9685
 * communication, allowing for dependency injection and easier unit testing. By
 * implementing this interface, concrete classes can provide either real
 * hardware communication or mock implementations for testing.
 *
 * The interface wraps the essential PCA9685 operations into virtual methods
 * that can be overridden by concrete implementations. See PCA9685Driver for
 * more information about its methods
 */
class APCA9685Driver
{
    public:
        APCA9685Driver() = default;
        virtual ~APCA9685Driver() = default;

        virtual void setRegister(uint8_t reg, uint8_t value) = 0;
        virtual void setPWMFrequency(float freq_hz) = 0;
        virtual void setPWMDutyCycle(uint8_t channel, uint16_t on,
                                     uint16_t off) = 0;
        virtual void setGPIO(uint8_t channel, bool on) = 0;
};
