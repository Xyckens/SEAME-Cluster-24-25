#pragma once

#include "CanInterface.hpp"
#include "MockCanDriver.hpp"
#include <gmock/gmock.h>
#include <rclcpp/rclcpp.hpp>
#include <thread>

class CanInterfaceTest : public ::testing::Test
{
    protected:
        void SetUp() override;
        void TearDown() override;

        std::shared_ptr<CanInterface> test_node_;
        rclcpp::executors::SingleThreadedExecutor::UniquePtr executor_;
        std::shared_ptr<MockCanDriver> mock_driver_;
        rclcpp::Client<custom_msgs::srv::CanService>::SharedPtr can_client_;
        std::thread executor_thread_;
};
