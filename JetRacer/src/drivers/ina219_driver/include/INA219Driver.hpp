#pragma once

#include "ADriver.hpp"
#include "std_msgs/msg/float64.hpp"
#include <custom_msgs/srv/i2c_service.hpp>
#include <rclcpp/rclcpp.hpp>

class INA219Driver : public ADriver
{
    public:
        INA219Driver(std::shared_ptr<rclcpp::Node> node,
                     uint8_t device_address);
        ~INA219Driver() = default;

        void publishBusVoltage();

    private:
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_voltage_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_perc_;

        uint32_t current_divider_mA_;
        uint32_t calibration_value_;
        float power_multiplier_mW_;

        // Map of callbacks for tracking responses, maps a register with the
        // response.
        using ReadCallback = std::function<void(std::vector<uint8_t>)>;
        std::unordered_map<uint8_t, ReadCallback> read_callbacks_;

        void setCalibration_32V_1A();
        void writeRegister(uint8_t reg, uint16_t value);
        void readRegister(uint8_t reg, uint8_t length, ReadCallback callback);
        void handleI2cReadResponse(
            rclcpp::Client<custom_msgs::srv::I2cService>::SharedFuture response,
            uint8_t reg);
};
