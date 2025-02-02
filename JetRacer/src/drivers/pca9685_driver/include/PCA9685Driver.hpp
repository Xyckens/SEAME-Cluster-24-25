#pragma once

#include <ADriver.hpp>
#include <APCA9685Driver.hpp>
#include <DriverException.hpp>
#include <custom_msgs/srv/i2c_service.hpp>
#include <rclcpp/rclcpp.hpp>

constexpr uint8_t PRESCALE_REGISTER = 0xFE;
constexpr uint8_t MODE1_REGISTER = 0x00;
constexpr uint8_t CHANNEL_REGISTER = 0x06;
constexpr uint8_t DEFAULT_CHANNEL = 0x00;

constexpr uint8_t MAX_CHANNEL = 15;
constexpr uint8_t LSB_MASK = 0xFF;
constexpr uint8_t SLEEP_MODE = 0x10;
constexpr uint8_t AUTO_INCREMENT = 0x80;

constexpr int CRISTAL_FREQUENCY = 25000000;
constexpr int NB_STEPS = 4096;

/**
 * @class PCA9685Driver
 * @brief Driver to interact with the PCA9685 (motor control)
 */
class PCA9685Driver : public ADriver, public APCA9685Driver
{
    public:
        PCA9685Driver(std::shared_ptr<rclcpp::Node> node,
                      uint8_t device_address);
        ~PCA9685Driver() override = default;

        void setRegister(uint8_t reg, uint8_t value) override;
        void setPWMFrequency(float freq_hz) override;
        void setPWMDutyCycle(uint8_t channel, uint16_t on,
                             uint16_t off) override;
        void setGPIO(uint8_t channel, bool on) override;
};
