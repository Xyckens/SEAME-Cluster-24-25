#pragma once

#include "APCA9685Driver.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>

constexpr int MIN_COUNT = 410;  // 10% duty cycle
constexpr int MAX_COUNT = 3680; // 90% duty cycle
constexpr int PCA_MOTORS_ADDRESS = 0x60;

constexpr int NODE_QOS = 10;
constexpr int MOTOR_FREQ = 1600;

/**
 * @class DcMotorsNode
 * @brief Listens to twist message and translate them into PWM signal values
 * that the PCA9685 driver understands.
 */
class DcMotorsNode : public rclcpp::Node
{
    public:
        DcMotorsNode();
        ~DcMotorsNode() override;

        uint8_t
        initPCA9685(std::shared_ptr<APCA9685Driver> mock_driver = nullptr);

    private:
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr
            twist_subscriber_;
        std::shared_ptr<APCA9685Driver> pca_driver_;

        void writeSpeed(geometry_msgs::msg::Twist::SharedPtr twist_msg);
};
