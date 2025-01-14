#include "DcMotorsNode.hpp"
#include <functional>
#include <rclcpp/client.hpp>
#include <rclcpp/logger.hpp>
#include <rclcpp/logging.hpp>

using namespace std::chrono_literals;

DcMotorsNode::DcMotorsNode() : Node("dc_motors_node")
{
    twist_subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "cmd_vel", 10,
        std::bind(&DcMotorsNode::writeSpeed, this, std::placeholders::_1));
}

DcMotorsNode::~DcMotorsNode() {}

uint8_t DcMotorsNode::initPCA9685()
{
    try
    {
        pca9685_ = std::make_shared<PCA9685Driver>(shared_from_this(),
                                                   PCA_MOTORS_ADDRESS);
    }
    catch (const std::exception& e)
    {
        RCLCPP_ERROR(this->get_logger(), "%s", e.what());
        return EXIT_FAILURE;
    }

    pca9685_->setPWMFrequency(1600);
    return EXIT_SUCCESS;
}

/**
 * @brief map speed in percentage to the appropriate duty cycle.
 *
 * freq = 1600;
 * 1 cycle = 1 /1600 = 625us
 * 625/4096 = 0.1526us per tick
 *
 * @param speed
 */
void DcMotorsNode::writeSpeed(
    const geometry_msgs::msg::Twist::SharedPtr twist_msg)
{
    float linear_x = twist_msg->linear.x;

    if (linear_x > 1.0 || linear_x < -1.0)
    {
        RCLCPP_ERROR(this->get_logger(),
                     "Invalid speed: %f (must be between -1.0 and 1.0)",
                     linear_x);
        return;
    }
    int8_t direction = (linear_x > 0) ? 1
                       : (linear_x < 0)
                           ? -1
                           : 0; // 1 = forward, -1 = reverse, 0 = stop

    linear_x = std::abs(linear_x);

    //  Map the linear velocity to PCA9685 pulse width
    uint16_t pulseWidth =
        static_cast<uint16_t>(linear_x * (MAX_COUNT - MIN_COUNT));

    pca9685_->setPWMDutyCycle(DEFAULT_CHANNEL, true, pulseWidth);
    pca9685_->setPWMDutyCycle(7, true, pulseWidth);

    if (direction > 0) // Forward
    {
        pca9685_->setGPIO(1, false);
        pca9685_->setGPIO(2, true);
        pca9685_->setGPIO(6, false);
        pca9685_->setGPIO(5, true);
    }
    else if (direction < 0) // Reverse
    {
        pca9685_->setGPIO(1, true);
        pca9685_->setGPIO(2, false);
        pca9685_->setGPIO(6, true);
        pca9685_->setGPIO(5, false);
    }
    else // Stop
    {
        // Set all GPIOs for both motors to neutral/off
        pca9685_->setGPIO(1, false);
        pca9685_->setGPIO(2, false);
        pca9685_->setGPIO(6, false);
        pca9685_->setGPIO(5, false);
    }
}
