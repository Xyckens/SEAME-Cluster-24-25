#pragma once

#include "II2cDriver.hpp"
#include <custom_msgs/srv/i2c_service.hpp>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/service.hpp>

constexpr size_t QUEUE_SIZE = 60;

/**
 * @class I2cInterface
 * @brief Handles read/write on the i2c bus via the I2C driver.
 *
 */
class I2cInterface : public rclcpp::Node
{
    public:
        I2cInterface(std::shared_ptr<II2cDriver> mock_driver = nullptr,
                     const std::string& service_name = "i2c_service");
        ~I2cInterface() override = default;

    private:
        std::shared_ptr<II2cDriver> i2c_driver_;
        rclcpp::Service<custom_msgs::srv::I2cService>::SharedPtr i2c_service_;

        void init();

        void handleI2cRequest(
            std::shared_ptr<custom_msgs::srv::I2cService::Request> request,
            std::shared_ptr<custom_msgs::srv::I2cService::Response> response);
};
