#pragma once

#include "I2cInterface.hpp"
#include "MockI2cDriver.hpp"
#include <custom_msgs/srv/i2c_service.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <rclcpp/rclcpp.hpp>
#include <thread>

class I2cInterfaceTest : public ::testing::Test
{
    protected:
        void SetUp() override;
        void TearDown() override;

        rclcpp::Client<custom_msgs::srv::I2cService>::SharedPtr i2c_client_;
        std::shared_ptr<I2cInterface> test_node_;
        rclcpp::executors::SingleThreadedExecutor::UniquePtr executor_;
        std::thread executor_thread_;
        std::shared_ptr<MockI2cDriver> mock_device_;
};
