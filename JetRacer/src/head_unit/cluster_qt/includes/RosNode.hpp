#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/u_int32.hpp>
#include <std_msgs/msg/u_int8.hpp>

enum class blinkerState
{
    IDLE,
    TURN_RIGHT,
    TURN_LEFT,
    WARNINGS,
};

/**
 * @class RosNode
 * @brief Subscribes to the battery level and speed topics for
 * the QT app to use
 */
class RosNode : public rclcpp::Node
{
    public:
        RosNode();
        ~RosNode() override = default;

        int getSpeed() const;
        int getBattery() const;
        int getRpm() const;
        blinkerState getBlinkerState() const;

    private:
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr battery_sub_;
        rclcpp::Subscription<std_msgs::msg::UInt8>::SharedPtr speed_sub_;
        rclcpp::Subscription<std_msgs::msg::UInt32>::SharedPtr rpm_sub_;
        rclcpp::Subscription<std_msgs::msg::UInt8>::SharedPtr blinker_sub_;

        void setBlinkerState(std_msgs::msg::UInt8 msg);

        int battery_level_{0};
        int speed_{0};
        int rpm_{0};
        blinkerState blinker_state_{blinkerState::IDLE};
};
