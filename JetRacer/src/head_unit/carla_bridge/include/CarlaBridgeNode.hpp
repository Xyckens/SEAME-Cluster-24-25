#pragma once

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <carla_msgs/msg/carla_ego_vehicle_control.hpp>


class CarlaBridgeNode : public rclcpp::Node
{
public:
    CarlaBridgeNode();

private:
    void twistCallback(geometry_msgs::msg::Twist::SharedPtr twist_msg);

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr twist_subscriber_;
    rclcpp::Publisher<carla_msgs::msg::CarlaEgoVehicleControl>::SharedPtr carla_control_publisher_;
};
