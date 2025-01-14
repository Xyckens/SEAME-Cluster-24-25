#pragma once

#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/u_int8.hpp>

#define IDLE 1
#define TURN_LEFT 2
#define TURN_RIGHT 3
#define WARNINGS 4

class TeleopNode : public rclcpp::Node
{
    public:
        TeleopNode();
        ~TeleopNode();

    private:
        rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_subscriber_;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_publisher_;
        rclcpp::Publisher<std_msgs::msg::UInt8>::SharedPtr blinkers_publisher_;

        void joyCallback(sensor_msgs::msg::Joy::SharedPtr joy_msg);
        void publishBlinkerState(sensor_msgs::msg::Joy::SharedPtr joy_msg);
};
