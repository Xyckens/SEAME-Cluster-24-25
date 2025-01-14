#include "../include/RosNode.hpp"

RosNode::RosNode() : rclcpp::Node("ros_node"), battery_level_(0), speed_(0)
{
    battery_sub_ = this->create_subscription<std_msgs::msg::Float64>(
        "battery_percentage", 10, [this](std_msgs::msg::Float64 battery_level)
        { battery_level_ = battery_level.data; });

    speed_sub_ = this->create_subscription<std_msgs::msg::UInt8>(
        "speed_sensor_readings", 10, [this](std_msgs::msg::UInt8 battery_level)
        { battery_level_ = battery_level.data; });
}
