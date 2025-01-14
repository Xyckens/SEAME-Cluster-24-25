#include "TeleopNode.hpp"

TeleopNode::TeleopNode() : rclcpp::Node("teleop_node")
{
    rclcpp::QoS qos(20);
    qos.reliable();

    joy_subscriber_ = this->create_subscription<sensor_msgs::msg::Joy>(
        "/joy", qos,
        std::bind(&TeleopNode::joyCallback, this, std::placeholders::_1));

    vel_publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    blinkers_publisher_ =
        this->create_publisher<std_msgs::msg::UInt8>("cmd_blinkers", 10);
}

TeleopNode::~TeleopNode() {}

void TeleopNode::joyCallback(sensor_msgs::msg::Joy::SharedPtr joy_msg)
{
    auto twist_msg = geometry_msgs::msg::Twist();

    twist_msg.linear.x = joy_msg->axes[1];  // Left stick, vertical axis
    twist_msg.angular.z = joy_msg->axes[0]; // Left stick, horizontal axis

    // Publish the Twist message to /cmd_vel
    vel_publisher_->publish(twist_msg);
    publishBlinkerState(joy_msg);
}

void TeleopNode::publishBlinkerState(sensor_msgs::msg::Joy::SharedPtr joy_msg)
{
    auto buttons = joy_msg->buttons;
    auto msg = std_msgs::msg::UInt8();

    if (buttons[0] == 1)
        blinkers_publisher_->publish(msg.set__data(IDLE));
    if (buttons[1] == 1)
        blinkers_publisher_->publish(msg.set__data(TURN_RIGHT));
    if (buttons[2] == 1)
        blinkers_publisher_->publish(msg.set__data(WARNINGS));
    if (buttons[3] == 1)
        blinkers_publisher_->publish(msg.set__data(TURN_LEFT));
}
