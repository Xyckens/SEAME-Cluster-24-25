#pragma once

#include "PCA9685Driver.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>

#define MIN_COUNT 410  // 10% duty cycle
#define MAX_COUNT 3680 // 90% duty cycle
#define PCA_MOTORS_ADDRESS 0x60

class DcMotorsNode : public rclcpp::Node
{
    public:
        DcMotorsNode();
        ~DcMotorsNode();

        uint8_t initPCA9685();

    private:
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr
            twist_subscriber_;
        std::shared_ptr<PCA9685Driver> pca9685_;

        void writeSpeed(const geometry_msgs::msg::Twist::SharedPtr twist_msg);
};
