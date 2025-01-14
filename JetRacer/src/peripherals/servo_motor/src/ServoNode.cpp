#include "ServoNode.hpp"
#include <functional>
#include <rclcpp/client.hpp>
#include <rclcpp/logger.hpp>
#include <rclcpp/logging.hpp>

using namespace std::chrono_literals;

ServoNode::ServoNode() : Node("servo_node")
{
    direction_subscriber_ =
        this->create_subscription<geometry_msgs::msg::Twist>(
            "cmd_vel", 10,
            std::bind(&ServoNode::writeAngle, this, std::placeholders::_1));
}

ServoNode::~ServoNode() {}

uint8_t ServoNode::initPCA9685()
{
    try
    {
        pca9685_ = std::make_shared<PCA9685Driver>(shared_from_this(),
                                                   PCA_SERVO_ADDRESS);
    }
    catch (const std::exception& e)
    {
        RCLCPP_ERROR(this->get_logger(), "%s", e.what());
        return EXIT_FAILURE;
    }
    pca9685_->setPWMFrequency(50);
    return EXIT_SUCCESS;
}

/**
 * @brief Handles incoming direction messages and maps the angle to PCA9685 PWM
 * counts, then sends the calculated duty cycle to the I2C service.
 *
 * This function subscribes to a topic providing direction as an angle (0 to 180
 * degrees). It validates the input and calculates the corresponding pulse width
 * for a servo motor.
 *
 * The PCA9685 divides the 20 ms (50 Hz) PWM period into 4096 discrete counts,
 * where each count corresponds to approximately 4.88 microseconds.
 * Servo motors typically require pulse widths between 1 ms (0°) and 2 ms
 * (180°), which map to 102 and 510 counts, respectively.
 *
 * The pulse width is linearly interpolated using the formula:
 * pulseWidth = MIN_COUNT + (angle * (MAX_COUNT - MIN_COUNT)) / 180
 *
 * For example:
 * - At 0 degrees: pulseWidth = 102 counts (HIGH for ~1 ms).
 * - At 180 degrees: pulseWidth = 510 counts (HIGH for ~2 ms).
 *
 * The `setPWM` function configures the PCA9685 as follows:
 * - onTime = 0 (signal starts HIGH at the beginning of the period).
 * - offTime = pulseWidth (signal goes LOW after `pulseWidth` counts).
 *
 * @param direction Shared pointer to the incoming UInt8 message containing the
 * angle.
 */
void ServoNode::writeAngle(const geometry_msgs::msg::Twist::SharedPtr twist)
{
    float angular_z = twist->angular.z;
    if (angular_z > 1.0 || angular_z < -1.0)
    {
        RCLCPP_ERROR(this->get_logger(),
                     "Invalid angular twist: %f (must be between 0 and 180)",
                     angular_z);
        return;
    }

    // map to an angle
    uint8_t angle = static_cast<uint8_t>((-angular_z + 1.0) * 90);

    RCLCPP_DEBUG(this->get_logger(), "Writing angle: %d", angle);

    // Map the angle (0 to 180) to PCA9685 pulse width (102 to 510)
    uint16_t pulseWidth = static_cast<uint16_t>(
        MIN_COUNT +
        (static_cast<float>(angle) * (MAX_COUNT - MIN_COUNT)) / 180);

    // Set the PWM signal using the adapted values
    pca9685_->setPWMDutyCycle(DEFAULT_CHANNEL, 0, pulseWidth);
}
