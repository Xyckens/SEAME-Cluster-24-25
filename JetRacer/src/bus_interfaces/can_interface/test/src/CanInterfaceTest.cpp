#include "CanInterfaceTest.hpp"

using namespace testing;

void CanInterfaceTest::SetUp()
{
    rclcpp::init(0, nullptr);

    mock_driver_ = std::make_shared<MockCanDriver>();
    test_node_ = std::make_shared<CanInterface>(mock_driver_);
    can_client_ =
        test_node_->create_client<custom_msgs::srv::CanService>("can_service");
    executor_ = rclcpp::executors::SingleThreadedExecutor::make_unique();
    executor_->add_node(test_node_);

    executor_thread_ = std::thread([this]() { executor_->spin(); });
}

void CanInterfaceTest::TearDown()
{
    executor_->cancel();
    if (executor_thread_.joinable())
        executor_thread_.join();
    rclcpp::shutdown();
}

// READ
TEST_F(CanInterfaceTest, readSuccess)
{
    EXPECT_CALL(*mock_driver_, waitForMessages).WillOnce(Return(true));
    EXPECT_CALL(*mock_driver_, readMessage)
        .WillOnce(
            []()
            {
                const sockcanpp::CanId id(0x10);
                sockcanpp::CanMessage msg(id, "hey");

                return msg;
            });

    auto request = std::make_shared<custom_msgs::srv::CanService::Request>();
    request->read_request = true;

    auto future = can_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, true);
    EXPECT_EQ(response->read_data.at(0), 'h');
}

TEST_F(CanInterfaceTest, readFailure)
{
    EXPECT_CALL(*mock_driver_, waitForMessages).WillOnce(Return(false));

    auto request = std::make_shared<custom_msgs::srv::CanService::Request>();
    request->read_request = true;

    auto future = can_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, false);
}

// WRITE
TEST_F(CanInterfaceTest, writeFailure)
{
    EXPECT_CALL(*mock_driver_, sendMessage)
        .WillOnce(
            []()
            {
                throw sockcanpp::exception();
                return 1;
            });

    auto request = std::make_shared<custom_msgs::srv::CanService::Request>();
    request->can_id = 0x10;
    request->write_data.push_back(0x30);

    auto future = can_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, false);
}
