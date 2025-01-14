#include "CanInterface.hpp"
#include "CanMessage.hpp"
#include <RCanDriver.hpp>
#include <linux/can.h>
#include <memory>
#include <rclcpp/logger.hpp>
#include <rclcpp/logging.hpp>
#include <rclcpp/utilities.hpp>

using sockcanpp::CanMessage;
using namespace std::chrono_literals;

CanInterface::CanInterface(std::shared_ptr<ICanDriver> can_driver)
    : Node("can_interface")
{
    rclcpp::QoS qos(60);
    qos.reliable();

    if (can_driver)
    {
        can_driver_ = can_driver;
        RCLCPP_INFO(this->get_logger(), "Mock driver received!");
    }
    else
    {
        try
        {
            can_driver_ = std::make_shared<RCanDriver>("can0", CAN_RAW);
        }
        catch (const std::exception& e)
        {
            RCLCPP_ERROR(this->get_logger(),
                         "Fail initiating Can interface: %s", e.what());
            throw e;
        }
    }

    can_service_ = this->create_service<custom_msgs::srv::CanService>(
        "can_service",
        std::bind(&CanInterface::handleCanRequest, this, std::placeholders::_1,
                  std::placeholders::_2),
        qos);

    RCLCPP_INFO(this->get_logger(), "Starting CAN bus interface");
}

CanInterface::~CanInterface() {}

/**
 * @brief function beeing called upon a request to the can service.
 *
 * the struct of the can service allows writting and reading operation. To see
 * the service definition do "ros2 interface show custom_msgs/srv/CanService" or
 * see JetRacer/src/bus_interfaces/custom_msgs/srv/CanService.
 *
 *
 * @param request
 * @param response
 */
void CanInterface::handleCanRequest(
    const std::shared_ptr<custom_msgs::srv::CanService::Request> request,
    std::shared_ptr<custom_msgs::srv::CanService::Response> response)
{
    response->set__success(true);

    if (!request->write_data.empty())
    {
        can_frame frame;

        frame.can_id = request->can_id;
        frame.can_dlc = request->write_data.size();
        auto data = request->write_data;
        for (size_t i = 0; i < frame.can_dlc; i++)
            frame.data[i] = data[0];

        try
        {
            can_driver_->sendMessage(frame);
            RCLCPP_DEBUG(this->get_logger(), "wrote frame to the bus");
        }
        catch (const std::exception& e)
        {
            response->set__success(false);
            response->set__message(e.what());
            RCLCPP_ERROR(this->get_logger(), "Fail writing data: %s", e.what());
            return;
        }
        // TODO: Incomplete writes
    }

    if (request->read_request)
    {
        if (!can_driver_->waitForMessages(50ms))
        {
            response->set__success(false);
            response->set__message("request timedout");
            RCLCPP_ERROR(this->get_logger(),
                         "Fail reading can bus, request timed out");
            return;
        }

        CanMessage received_msg = can_driver_->readMessage();
        auto frame_data = received_msg.getFrameData();

        // Resize the response read_data to match the size of frame_data
        response->read_data.resize(frame_data.size());
        std::copy(frame_data.begin(), frame_data.end(),
                  response->read_data.begin());
    }
    response->set__message("success");
}
