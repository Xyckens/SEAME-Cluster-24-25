#pragma once

#include "ADriver.hpp"
#include <cstdint>
#include <custom_msgs/msg/display.hpp>
#include <custom_msgs/srv/i2c_service.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

/**
 * @class SSD1306Driver
 * @brief Driver to interact with the SSD1306 (Oled screen)
 *
 */
class SSD1306Driver : public ADriver
{
    public:
        SSD1306Driver(std::shared_ptr<rclcpp::Node> node);
        ~SSD1306Driver() override;

        // display interface
        void initDisplay();
        void setDefaultConfig();
        void onOffDisplay(uint8_t onoff);
        void setCursor(uint8_t x, uint8_t y);
        void clearRow(uint8_t row);
        void clearScreen();
        void writeString(uint8_t size, const std::string& msg);
        void flipDisplay(uint8_t flip);
        void rotateDisplay(uint8_t degree);
};
