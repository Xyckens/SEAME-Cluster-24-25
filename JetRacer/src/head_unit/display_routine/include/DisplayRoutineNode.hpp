#pragma once

#include <custom_msgs/msg/display.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/string.hpp>

class DisplayRoutineNode : public rclcpp::Node
{
    public:
        DisplayRoutineNode();
        ~DisplayRoutineNode() = default;

    private:
        rclcpp::Publisher<custom_msgs::msg::Display>::SharedPtr publisher_;
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr
            battery_subscriber_;
        rclcpp::TimerBase::SharedPtr timer_;

        std::string ip_address_;
        std::string battery_level_;

        void updateDisplay() const;
        std::vector<std::string> getIPv4Addresses() const;
};
