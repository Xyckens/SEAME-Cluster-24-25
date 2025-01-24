#pragma once

#include "APCA9685Driver.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/client.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/subscription.hpp>
#include <std_msgs/msg/u_int8.hpp>

#define MIN_COUNT 102
#define MAX_COUNT 510
#define PCA_SERVO_ADDRESS 0x40

/**
 * @class ServoNode
 * @brief Listens to twist message and translate them into PWM signal values
 * that the pca driver understands.
 */
class ServoNode : public rclcpp::Node
{
    public:
        ServoNode();
        ~ServoNode();

        uint8_t
        initPCA9685(std::shared_ptr<APCA9685Driver> mock_pca_driver = nullptr);

    private:
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr
            direction_subscriber_;
        std::shared_ptr<APCA9685Driver> pca_driver_;

        void writeAngle(const geometry_msgs::msg::Twist::SharedPtr twist);

        const float_t MAX_ANGULAR_VEL = 1.0;
};
