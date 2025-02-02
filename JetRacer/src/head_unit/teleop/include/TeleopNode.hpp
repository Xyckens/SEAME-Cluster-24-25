#pragma once

#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <std_msgs/msg/u_int8.hpp>

enum class BlinkerState
{
    IDLE,
    TURN_LEFT,
    TURN_RIGHT,
    WARNINGS,

};

constexpr int NODE_QOS = 10;
constexpr int QUEUE_DEPTH = 20;

/**
 * @class TeleopNode
 * @brief Subscribes to the joystick topic, convert it and publishes it into
 * cmd_vel topic.
 *
 * This class essentialy converts joy_msg into twist_msg. It also publishes the
 * blinker state based on the joystick buttons state.
 */
class TeleopNode : public rclcpp::Node
{
    public:
        TeleopNode();
        ~TeleopNode() override;

    private:
        rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_subscriber_;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_publisher_;
        rclcpp::Publisher<std_msgs::msg::UInt8>::SharedPtr blinkers_publisher_;

        void joyCallback(sensor_msgs::msg::Joy::SharedPtr joy_msg);
        void publishBlinkerState(sensor_msgs::msg::Joy::SharedPtr joy_msg);
};
