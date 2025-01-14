#pragma once

#include <custom_msgs/srv/i2c_service.hpp>
#include <rclcpp/rclcpp.hpp>

class ADriver
{
    public:
        ADriver(rclcpp::Node::SharedPtr node, uint8_t device_address);
        virtual ~ADriver();

    protected:
        rclcpp::Node::SharedPtr node_;
        rclcpp::Client<custom_msgs::srv::I2cService>::SharedPtr i2c_client_;
        uint8_t device_address_;

        void ping();
        virtual void handleI2cResponse(
            rclcpp::Client<custom_msgs::srv::I2cService>::SharedFuture
                response);
};

class DriverException : public std::exception
{
    private:
        std::string msg_;

    public:
        DriverException(const std::string& msg) : msg_(msg) {}
        const char* what() const noexcept override { return msg_.c_str(); }
};
