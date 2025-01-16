#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/u_int8.hpp>

class RosNode : public rclcpp::Node
{
    public:
        RosNode();
        ~RosNode() = default;

        int getSpeed() const;
        int getBattery() const;

    private:
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr battery_sub_;
        rclcpp::Subscription<std_msgs::msg::UInt8>::SharedPtr speed_sub_;
        // TODO: add subscriber for cmd_vel (see the definition of Twist to
        // see what field to refer to)
        // definition:
        // Vector3  linear
        // Vector3  angular
        // only the field x is used in linear (forward movement)
        // only the field z i used in angular (turning)

        int battery_level_;
        int speed_;
};
