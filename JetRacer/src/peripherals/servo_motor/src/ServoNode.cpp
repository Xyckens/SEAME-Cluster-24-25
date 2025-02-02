#include "ServoNode.hpp"
#include <PCA9685Driver.hpp>
#include <algorithm>
#include <functional>
#include <rclcpp/client.hpp>
#include <rclcpp/logger.hpp>
#include <rclcpp/logging.hpp>

using namespace std::chrono_literals;

ServoNode::ServoNode() : Node("servo_node")
{
    direction_subscriber_ =
        this->create_subscription<geometry_msgs::msg::Twist>(
            "cmd_vel", NODE_QOS,
            std::bind(&ServoNode::writeAngle, this, std::placeholders::_1));
}

ServoNode::~ServoNode() {}

/**
 * @brief Initialize the PCA9685 driver.
 *
 * @param mock_driver if not null, the driver will be mocked.
 * @return EXIT_FAILURE if the driver fails to initialize, EXIT_SUCCESS
 * otherwise.
 */
uint8_t ServoNode::initPCA9685(std::shared_ptr<APCA9685Driver> mock_driver)
{
    if (mock_driver)
        pca_driver_ = mock_driver;
    else
    {

        try
        {
            pca_driver_ = std::make_shared<PCA9685Driver>(shared_from_this(),
                                                          PCA_SERVO_ADDRESS);
        }
        catch (const std::exception& e)
        {
            RCLCPP_ERROR(this->get_logger(), "%s", e.what());
            return EXIT_FAILURE;
        }
    }
    pca_driver_->setPWMFrequency(SERVO_FREQ_HZ);
    return EXIT_SUCCESS;
}

/**
 * @brief Handles incoming direction messages and maps the angle to PCA9685 PWM
 * counts.
 *
 * This function subscribes to a topic providing direction as an angle (0 to 180
 * degrees). It validates the input and calculates the corresponding pulse width
 * for a servo motor.
 * The PCA9685 divides the 20 ms (50 Hz) PWM period into 4096 discrete counts,
 * where each count corresponds to approximately 4.88 microseconds.
 * Servo motors typically require pulse widths between 1 ms (0°) and 2 ms
 * (180°), which map to 102 and 510 counts, respectively.
 * The pulse width is linearly interpolated using the formula:
 * pulseWidth = MIN_COUNT + (angle * (MAX_COUNT - MIN_COUNT)) / 180
 * For example:
 * - At 0 degrees: pulseWidth = 102 counts (HIGH for ~1 ms).
 * - At 180 degrees: pulseWidth = 510 counts (HIGH for ~2 ms).
 * The `setPWM` function configures the PCA9685 as follows:
 * - onTime = 0 (signal starts HIGH at the beginning of the period).
 * - offTime = pulseWidth (signal goes LOW after `pulseWidth` counts).
 *
 * @param twist The incoming direction message.
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
    int angle = (-angular_z + 1.0) * CENTER_ANGLE;

    // restrain angle
    int restrained_angle =
        std::clamp((int)angle, MIN_REST_ANGLE, MAX_REST_ANGLE);
    RCLCPP_DEBUG(this->get_logger(), "Writing angle: %d", restrained_angle);

    // Map the angle (0 to 180) to PCA9685 pulse width (102 to 510)
    auto pulse_width = static_cast<uint16_t>(
        MIN_COUNT +
        (static_cast<float>(restrained_angle) * (MAX_COUNT - MIN_COUNT)) /
            MAX_ANGLE);

    pca_driver_->setPWMDutyCycle(DEFAULT_CHANNEL, 0, pulse_width);
}
