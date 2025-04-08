#include "SpeedSensorNode.hpp"
#include "std_msgs/msg/u_int8.hpp"
#include <rclcpp/logging.hpp>

SpeedSensorNode::SpeedSensorNode() : rclcpp::Node("speed_sensor")
{
    client_ = this->create_client<custom_msgs::srv::CanService>("can_service");
    timer_ = this->create_wall_timer(std::chrono::milliseconds(POLL_FREQ_MS),
                                [this]() { readSpeed(); });
    speed_publisher_ = this->create_publisher<std_msgs::msg::UInt8>(
        "speed_readings", NODE_QOS);
    rpm_publisher_ =
        this->create_publisher<std_msgs::msg::UInt32>("rpm_readings", NODE_QOS);
}

SpeedSensorNode::~SpeedSensorNode() {}

/**
 * @brief function called upon receiving a CAN_frame. reads the can_raw topic
 * and publishes the speed in a different topic
 *
 * @param future the future containing the response from the service
 */
void SpeedSensorNode::writeSpeed(
    rclcpp::Client<custom_msgs::srv::CanService>::SharedFuture future)
{
    auto response = future.get();
    if (!response->success)
        RCLCPP_ERROR_THROTTLE(this->get_logger(), *this->get_clock(),
                              ERROR_OUTPUT_FREQ, "%s",
                              response->message.c_str());
    else
    {
        std_msgs::msg::UInt8 speed_msg;
        std_msgs::msg::UInt32 rpm_msg;
        auto rot_per_sec =
            (response->read_data.at(0) / static_cast<float>(NB_HOLES)) *
            (MS_TO_S / static_cast<float>(POLL_FREQ_MS));
        float calculated_speed = (rot_per_sec * WHEELS_PERIMETER_M) *
                                 M_TO_DM; // calculated speed dm/s

        rpm_msg.set__data(rot_per_sec * SEC_TO_MIN);
        speed_msg.set__data(calculated_speed);

        speed_publisher_->publish(speed_msg);
        rpm_publisher_->publish(rpm_msg);
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
