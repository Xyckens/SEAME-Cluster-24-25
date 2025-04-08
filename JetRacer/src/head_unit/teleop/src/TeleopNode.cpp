#include "TeleopNode.hpp"

TeleopNode::TeleopNode() : rclcpp::Node("teleop_node")
{
    rclcpp::QoS qos(QUEUE_DEPTH);
    qos.reliable();

    joy_subscriber_ = this->create_subscription<sensor_msgs::msg::Joy>(
        "/joy", qos,
        std::bind(&TeleopNode::joyCallback, this, std::placeholders::_1));

    vel_publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", NODE_QOS);
    blinkers_publisher_ =
        this->create_publisher<std_msgs::msg::UInt8>("cmd_blinkers", NODE_QOS);
}

TeleopNode::~TeleopNode() {}

/**
 * @brief Callback function called upon receiving a message from the joystick
 *
 * @param joy_msg The joystick message
 */
void TeleopNode::joyCallback(sensor_msgs::msg::Joy::SharedPtr joy_msg)
{
    auto twist_msg = geometry_msgs::msg::Twist();

    twist_msg.linear.x = joy_msg->axes[1];  // Left stick, vertical axis
    twist_msg.angular.z = joy_msg->axes[0]; // Left stick, horizontal axis

    // Publish the Twist message to /cmd_vel
    vel_publisher_->publish(twist_msg);
    publishBlinkerState(joy_msg);
}

/**
 * @brief Publishes the blinker state based on the joystick buttons state
 *
 * @param joy_msg The joystick message
 */
void TeleopNode::publishBlinkerState(sensor_msgs::msg::Joy::SharedPtr joy_msg)
{
    auto buttons = joy_msg->buttons;
    auto msg = std_msgs::msg::UInt8();

    if (buttons[0] == 1)
        blinkers_publisher_->publish(
            msg.set__data(static_cast<uint8_t>(BlinkerState::IDLE)));
    if (buttons[1] == 1)
        blinkers_publisher_->publish(
            msg.set__data(static_cast<uint8_t>(BlinkerState::TURN_RIGHT)));
    if (buttons[4] == 1)
        blinkers_publisher_->publish(
            msg.set__data(static_cast<uint8_t>(BlinkerState::WARNINGS)));
    if (buttons[3] == 1)
        blinkers_publisher_->publish(
            msg.set__data(static_cast<uint8_t>(BlinkerState::TURN_LEFT)));
}
