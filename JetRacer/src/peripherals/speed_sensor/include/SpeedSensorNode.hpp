#pragma once

#include "custom_msgs/msg/can_frame.hpp"
#include "std_msgs/msg/u_int8.hpp"
#include <cmath>
#include <custom_msgs/srv/can_service.hpp>
#include <rclcpp/client.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/subscription.hpp>
#include <std_msgs/msg/u_int32.hpp>
#include <std_msgs/msg/u_int8.hpp>

// NODE
constexpr int SPEED_SENSOR_ID = 0x300;
constexpr int NODE_QOS = 10;
constexpr int ERROR_OUTPUT_FREQ = 3000;
constexpr int POLL_FREQ_MS = 100;
constexpr int NB_HOLES = 36;
constexpr float WHEELS_DIAM_M = 0.065;
constexpr float WHEELS_PERIMETER_M = M_PI * (WHEELS_DIAM_M);

// CONVERSIONS
constexpr int MS_TO_S = 1000;
constexpr int M_TO_DM = 10;
constexpr int SEC_TO_MIN = 60;

/**
 * @class SpeedSensorNode
 * @brief Reads speed from the CAN bus and publishes it into
 * speed_readings and rpm_readings topics
 */
class SpeedSensorNode : public rclcpp::Node
{
    public:
        SpeedSensorNode();
        ~SpeedSensorNode() override;

    private:
        rclcpp::Subscription<custom_msgs::msg::CanFrame>::SharedPtr
            raw_can_subscriber_;
        rclcpp::Publisher<std_msgs::msg::UInt8>::SharedPtr speed_publisher_;
        rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr rpm_publisher_;
        rclcpp::Client<custom_msgs::srv::CanService>::SharedPtr client_;
        rclcpp::TimerBase::SharedPtr timer_;

        void writeSpeed(
            rclcpp::Client<custom_msgs::srv::CanService>::SharedFuture future);
        void readSpeed();
};
