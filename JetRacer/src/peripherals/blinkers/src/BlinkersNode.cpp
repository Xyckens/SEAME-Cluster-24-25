#include "BlinkersNode.hpp"
#include "std_msgs/msg/u_int8.hpp"
#include <rclcpp/logging.hpp>

using namespace std::chrono_literals;

BlinkersNode::BlinkersNode() : Node("blinkers_node")
{
    can_client_ =
        this->create_client<custom_msgs::srv::CanService>("can_service");
    blinker_state_subscriber_ = this->create_subscription<std_msgs::msg::UInt8>(
        "cmd_blinkers", 10,
        std::bind(&BlinkersNode::writeBlinkersState, this,
                  std::placeholders::_1));
}

void BlinkersNode::writeBlinkersState(const std_msgs::msg::UInt8 msg)
{
    auto request = std::make_shared<custom_msgs::srv::CanService::Request>();

    request->set__can_id(BLINKERS_STATE_ID);
    request->set__read_request(false);
    request->write_data.push_back(msg.data);

    can_client_->async_send_request(request,
                                    std::bind(&BlinkersNode::handleCanResponse,
                                              this, std::placeholders::_1));
}

void BlinkersNode::handleCanResponse(
    rclcpp::Client<custom_msgs::srv::CanService>::SharedFuture future)
{
    auto response = future.get();
    if (!response->success)
    {
        RCLCPP_ERROR(this->get_logger(), "%s", response->message.c_str());
    }
}
