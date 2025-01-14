#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/u_int8.hpp>

class RosNode : public rclcpp::Node
{
    public:
        RosNode();
        ~RosNode();

    private:
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr battery_sub_;
        rclcpp::Subscription<std_msgs::msg::UInt8>::SharedPtr speed_sub_;

        float_t battery_level_;
        uint8_t speed_;
};
