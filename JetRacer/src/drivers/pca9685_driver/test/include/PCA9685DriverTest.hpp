#pragma once

#include "MockI2cService.hpp"
#include <custom_msgs/srv/i2c_service.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <rclcpp/rclcpp.hpp>

class PCA9685DriverTest : public ::testing::Test
{
    protected:
        void SetUp() override;
        void TearDown() override;

        std::shared_ptr<rclcpp::Node> test_node_;
        std::shared_ptr<MockI2cServiceNode> mock_i2c_node_;
        std::shared_ptr<rclcpp::executors::SingleThreadedExecutor> executor_;
        std::thread executor_thread_;
};
