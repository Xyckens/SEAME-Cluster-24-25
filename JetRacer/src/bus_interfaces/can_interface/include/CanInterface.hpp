#pragma once

#include "ICanDriver.hpp"
#include "custom_msgs/srv/can_service.hpp"
#include <memory>
#include <rclcpp/rclcpp.hpp>

using sockcanpp::CanDriver;

class CanInterface : public rclcpp::Node
{
    public:
        CanInterface(std::shared_ptr<ICanDriver> can_driver = nullptr);
        ~CanInterface();

        void pollCanBus();

    private:
        std::shared_ptr<ICanDriver> can_driver_;
        rclcpp::Service<custom_msgs::srv::CanService>::SharedPtr can_service_;

        void handleCanRequest(
            const std::shared_ptr<custom_msgs::srv::CanService::Request>
                request,
            std::shared_ptr<custom_msgs::srv::CanService::Response> response);
};
