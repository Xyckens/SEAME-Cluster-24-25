#pragma once

#include "SSD1306Driver.hpp"
#include <custom_msgs/msg/display.hpp>
#include <custom_msgs/srv/i2c_service.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#define SSD1306_FONT_NORMAL 0x01

class OledDisplayNode : public rclcpp::Node
{
    public:
        OledDisplayNode();
        ~OledDisplayNode();

        uint8_t initSSD1306();

    private:
        rclcpp::Subscription<custom_msgs::msg::Display>::SharedPtr
            display_subscriber_;
        std::unique_ptr<SSD1306Driver> ssd1306_;

        void writeToI2c(const custom_msgs::msg::Display::SharedPtr msg);
};
