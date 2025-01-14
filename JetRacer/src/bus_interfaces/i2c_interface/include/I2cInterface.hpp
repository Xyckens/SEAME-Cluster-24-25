#pragma once

#include "I2cDriver.hpp"
#include "custom_msgs/srv/i2c_service.hpp"
#include <custom_msgs/srv/i2c_service.hpp>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/service.hpp>

class I2cInterface : public rclcpp::Node
{
    public:
        I2cInterface(std::shared_ptr<II2cDriver> i2c_dev = nullptr,
                     const std::string& service_name = "i2c_service");
        ~I2cInterface() = default;

    private:
        std::shared_ptr<II2cDriver> i2c_driver_;
        rclcpp::Service<custom_msgs::srv::I2cService>::SharedPtr i2c_service_;

        void init_();

        void handleI2cRequest(
            const std::shared_ptr<custom_msgs::srv::I2cService::Request>
                request,
            std::shared_ptr<custom_msgs::srv::I2cService::Response> response);
};
