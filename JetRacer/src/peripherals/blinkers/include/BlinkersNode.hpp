#pragma once

#include "std_msgs/msg/u_int8.hpp"
#include <custom_msgs/srv/can_service.hpp>
#include <rclcpp/client.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/subscription.hpp>
#include <std_msgs/msg/u_int8.hpp>

#define BLINKERS_STATE_ID 0x100
#define IDLE 1
#define TURN_LEFT 2
#define TURN_RIGHT 3
#define WARNINGS 4

/**
 * @class BlinkersNode
 * @brief listens to a topic dictating the state of the blinkers. upon changing
 * state, a can request will be sent to the arduino to update the blinkers.
 *
 */
class BlinkersNode : public rclcpp::Node
{
    public:
        BlinkersNode();
        ~BlinkersNode() = default;

    private:
        rclcpp::Subscription<std_msgs::msg::UInt8>::SharedPtr
            blinker_state_subscriber_;
        rclcpp::Client<custom_msgs::srv::CanService>::SharedPtr can_client_;

        void writeBlinkersState(const std_msgs::msg::UInt8 msg);
        void handleCanResponse(
            rclcpp::Client<custom_msgs::srv::CanService>::SharedFuture future);
};
