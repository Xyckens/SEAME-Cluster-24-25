#include "../include/CarlaBridgeNode.hpp"

CarlaBridgeNode::CarlaBridgeNode() : rclcpp::Node("carla_bridge_node")
{
    twist_subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "cmd_vel", 10,
        std::bind(&CarlaBridgeNode::twistCallback, this, std::placeholders::_1));

    carla_control_publisher_ = this->create_publisher<carla_msgs::msg::CarlaEgoVehicleControl>(
        "/carla/ego_vehicle/vehicle_control_cmd", 10);
}

void CarlaBridgeNode::twistCallback(geometry_msgs::msg::Twist::SharedPtr twist_msg)
{
    auto carla_control_msg = carla_msgs::msg::CarlaEgoVehicleControl();
    
    carla_control_msg.throttle = twist_msg->linear.x;
    carla_control_msg.brake = (twist_msg->linear.x < 0) ? -twist_msg->linear.x : 0.0;
    carla_control_msg.steer = twist_msg->angular.z;

    carla_control_publisher_->publish(carla_control_msg);
}