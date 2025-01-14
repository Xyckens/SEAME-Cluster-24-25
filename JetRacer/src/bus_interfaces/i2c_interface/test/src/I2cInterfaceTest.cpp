#include "I2cInterfaceTest.hpp"
#include <gtest/gtest.h>

using namespace std::chrono_literals;
using namespace testing;

void I2cInterfaceTest::SetUp()
{
    rclcpp::init(0, nullptr);

    mock_device_ = std::make_shared<MockI2cDriver>(1);
    executor_ = rclcpp::executors::SingleThreadedExecutor::make_unique();
    test_node_ = std::make_shared<I2cInterface>(mock_device_, "i2c_service2");
    executor_->add_node(test_node_);

    i2c_client_ =
        test_node_->create_client<custom_msgs::srv::I2cService>("i2c_service2");
    executor_thread_ = std::thread([this]() { executor_->spin(); });
}

void I2cInterfaceTest::TearDown()
{
    executor_->cancel();
    if (executor_thread_.joinable())
        executor_thread_.join();
    rclcpp::shutdown();
}

// READ OPERATIONS
TEST_F(I2cInterfaceTest, read1byte)
{
    EXPECT_CALL(*mock_device_, read)
        .WillOnce(
            [](std::vector<uint8_t>& buff)
            {
                EXPECT_EQ(buff.size(), 1);
                buff.at(0) = 0x10;
                return 1;
            });

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();
    request->device_address = 0x40;
    request->read_request = true;
    request->read_length = 1;

    auto future = i2c_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, true);
    EXPECT_EQ(response->read_data[0], 0x10);
}

TEST_F(I2cInterfaceTest, readFailure)
{
    EXPECT_CALL(*mock_device_, read).WillOnce(testing::Return(-1));

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();
    request->device_address = 0x40;
    request->read_request = true;
    request->read_length = 1;

    auto future = i2c_client_->async_send_request(request);
    auto response = future.get();

    ASSERT_EQ(response->success, false);
}

TEST_F(I2cInterfaceTest, readIncomplete)
{
    EXPECT_CALL(*mock_device_, read)
        .WillOnce([](std::vector<uint8_t>& buff) { return 1; });

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();
    request->device_address = 0x40;
    request->read_request = true;
    request->read_length = 3;

    auto future = i2c_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, false);
}

// WRITE OPERATIONS
TEST_F(I2cInterfaceTest, write1byte)
{
    EXPECT_CALL(*mock_device_, write)
        .WillOnce(
            [](std::vector<uint8_t>& buff)
            {
                EXPECT_EQ(buff.at(0), 0x10);
                return 1;
            });

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();
    request->device_address = 0x40;
    request->read_request = false;
    request->write_data.push_back(0x10);

    auto future = i2c_client_->async_send_request(request);
    auto response = future.get();

    ASSERT_EQ(response->success, true);
}

TEST_F(I2cInterfaceTest, writeFailure)
{
    EXPECT_CALL(*mock_device_, write).WillOnce(Return(-1));

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();
    request->device_address = 0x40;
    request->read_request = false;
    request->write_data.push_back(0x10);

    auto future = i2c_client_->async_send_request(request);
    auto response = future.get();

    ASSERT_EQ(response->success, false);
    ASSERT_FALSE(response->message.empty());
}

TEST_F(I2cInterfaceTest, writeIncomplete)
{
    EXPECT_CALL(*mock_device_, write).WillOnce(Return(1));

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();
    request->device_address = 0x40;
    request->read_request = false;
    request->write_data.insert(request->write_data.end(), {0x10, 0x11, 0x12});

    auto future = i2c_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, false);
}

// TODO: failures, incomplete read, timeouts, sequential reads..
//
TEST_F(I2cInterfaceTest, setAddressFailure)
{
    EXPECT_CALL(*mock_device_, setAddress).WillOnce(testing::Return(-1));

    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();
    request->device_address = 0x40;

    auto future = i2c_client_->async_send_request(request);
    auto response = future.get();

    ASSERT_EQ(response->success, false);
}
