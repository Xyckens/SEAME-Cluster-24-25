#pragma once

#include "I2cDevice.hpp"
#include "custom_msgs/srv/i2c_service.hpp"
#include <custom_msgs/srv/i2c_service.hpp>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/service.hpp>

class I2cInterface : public rclcpp::Node
{
    public:
        I2cInterface();
        ~I2cInterface() = default;

    private:
        I2cDevice i2c_device_;
        rclcpp::Service<custom_msgs::srv::I2cService>::SharedPtr i2c_service_;

        void init_();

        void handleI2cRequest(
            const std::shared_ptr<custom_msgs::srv::I2cService::Request>
                request,
            std::shared_ptr<custom_msgs::srv::I2cService::Response> response);
};
