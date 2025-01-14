#include "OledDisplayNode.hpp"

using namespace std::chrono_literals;

OledDisplayNode::OledDisplayNode() : rclcpp::Node("oled_display")
{
    display_subscriber_ = this->create_subscription<custom_msgs::msg::Display>(
        "cmd_display", 10,
        std::bind(&OledDisplayNode::writeToI2c, this, std::placeholders::_1));
}

OledDisplayNode::~OledDisplayNode() {}

uint8_t OledDisplayNode::initSSD1306()
{
    try
    {
        ssd1306_ = std::make_unique<SSD1306Driver>(shared_from_this());
    }
    catch (const std::exception& e)
    {
        RCLCPP_ERROR(this->get_logger(), "%s", e.what());
        return EXIT_FAILURE;
    }

    ssd1306_->initDisplay();
    ssd1306_->setDefaultConfig();
    ssd1306_->onOffDisplay(0);
    ssd1306_->clearScreen();
    ssd1306_->rotateDisplay(0);
    return EXIT_SUCCESS;
}

/**
 * @brief request to the i2c service a write operation via the driver upon
 * receiving a message in the display_topic
 *
 * @param msg
 */
void OledDisplayNode::writeToI2c(const custom_msgs::msg::Display::SharedPtr msg)
{
    ssd1306_->clearScreen();

    ssd1306_->setCursor(2, 0);
    ssd1306_->writeString(SSD1306_FONT_NORMAL, msg->line1);
    ssd1306_->setCursor(2, 1);
    ssd1306_->writeString(SSD1306_FONT_NORMAL, msg->line2);
    ssd1306_->setCursor(2, 2);
    ssd1306_->writeString(SSD1306_FONT_NORMAL, msg->line3);
    ssd1306_->setCursor(2, 3);
    ssd1306_->writeString(SSD1306_FONT_NORMAL, msg->line4);
}
