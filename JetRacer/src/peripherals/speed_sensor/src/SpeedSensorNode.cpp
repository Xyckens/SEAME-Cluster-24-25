#include "SpeedSensorNode.hpp"
#include "std_msgs/msg/u_int8.hpp"
#include <rclcpp/logging.hpp>

using namespace std::chrono_literals;

SpeedSensorNode::SpeedSensorNode() : rclcpp::Node("speed_sensor")
{
    client_ = this->create_client<custom_msgs::srv::CanService>("can_service");
    timer_ = this->create_timer(100ms, [this]() { readSpeed(); });
    speed_publisher_ = this->create_publisher<std_msgs::msg::UInt8>(
        "speed_sensor_readings", 10);
}

SpeedSensorNode::~SpeedSensorNode() {}

/**
 * @brief function called upon receiving a CAN_frame. reads the can_raw topic
 * and publishes the speed in a different topic
 *
 * @param can_frame
 */
void SpeedSensorNode::writeSpeed(
    rclcpp::Client<custom_msgs::srv::CanService>::SharedFuture future)
{
    auto response = future.get();
    if (!response->success)
        RCLCPP_ERROR(this->get_logger(), "%s", response->message.c_str());
    else
    {
        std_msgs::msg::UInt8 speed_msg;

        speed_msg.set__data(response->read_data.at(0));
        speed_publisher_->publish(speed_msg);
    }
}

/**
 * @brief request a read operation on the bus
 *
 * the response should contain the data requested
 */
void SpeedSensorNode::readSpeed()
{
    auto request = std::make_shared<custom_msgs::srv::CanService::Request>();

    request->set__can_id(SPEED_SENSOR_ID);
    request->set__read_request(true);
    request->set__write_data(std::vector<uint8_t>{1});
    client_->async_send_request(
        request,
        [this](
            rclcpp::Client<custom_msgs::srv::CanService>::SharedFuture future)
        { writeSpeed(future); });
}
