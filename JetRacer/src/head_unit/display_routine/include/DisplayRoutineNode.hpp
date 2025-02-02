#pragma once

#include <custom_msgs/msg/display.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/string.hpp>

constexpr int NODE_QOS = 10;
/**
 * @class DisplayRoutineNode
 * @brief Update the oled display.
 *
 * this node is responsible for updating the oled display with the current IP
 * address and battery with a 60s interval.
 */
class DisplayRoutineNode : public rclcpp::Node
{
    public:
        DisplayRoutineNode();
        ~DisplayRoutineNode() override = default;

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
