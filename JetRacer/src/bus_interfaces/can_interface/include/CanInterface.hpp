#pragma once

#include "ICanDriver.hpp"
#include "custom_msgs/srv/can_service.hpp"
#include <memory>
#include <rclcpp/rclcpp.hpp>

constexpr int QUEUE_SIZE = 60;
constexpr int ERROR_OUTPUT_FREQ = 5000;

using sockcanpp::CanDriver;

/**
 * @class CanInterface
 * @brief Handles read and write operation onto the can bus via the can driver.
 *
 */
class CanInterface : public rclcpp::Node
{
    public:
        CanInterface(std::shared_ptr<ICanDriver> mock_driver = nullptr);
        ~CanInterface() override;

        void pollCanBus();

    private:
        std::shared_ptr<ICanDriver> can_driver_;
        rclcpp::Service<custom_msgs::srv::CanService>::SharedPtr can_service_;

        void handleCanRequest(
            std::shared_ptr<custom_msgs::srv::CanService::Request> request,
            std::shared_ptr<custom_msgs::srv::CanService::Response> response);
};
