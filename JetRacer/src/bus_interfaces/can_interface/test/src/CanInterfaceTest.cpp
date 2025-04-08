#include "CanInterfaceTest.hpp"

using namespace testing;

constexpr uint8_t CAN_ID = 0x10;
constexpr char DATA[3] = {'h', 'e', 'y'};

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
                const sockcanpp::CanId id(CAN_ID);
                sockcanpp::CanMessage msg(id, {DATA[0]});

                return msg;
            });

    auto request = std::make_shared<custom_msgs::srv::CanService::Request>();
    request->read_request = true;

    auto future = can_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, true);
    EXPECT_EQ(response->read_data.at(0), DATA[0]);
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
TEST_F(CanInterfaceTest, writeSuccess)
{
    EXPECT_CALL(*mock_driver_, sendMessage)
        .WillOnce(
            [](const sockcanpp::CanMessage& msg, bool force_extended)
            {
                EXPECT_EQ(msg.getCanId(), 0x10);
                EXPECT_EQ(msg.getFrameData().size(), 3);
                EXPECT_EQ(msg.getFrameData().at(0), 'h');
                EXPECT_EQ(msg.getFrameData().at(2), 'y');

                return 3;
            });

    auto request = std::make_shared<custom_msgs::srv::CanService::Request>();
    request->read_request = false;
    request->can_id = CAN_ID;
    request->write_data.insert(request->write_data.end(), std::begin(DATA),
                               std::end(DATA));

    auto future = can_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, true);
}

TEST_F(CanInterfaceTest, writeFailure)
{
    EXPECT_CALL(*mock_driver_, sendMessage)
        .WillOnce(
            [](const sockcanpp::CanMessage& msg, bool force_extended)
            {
                throw sockcanpp::exception();
                return 1;
            });

    auto request = std::make_shared<custom_msgs::srv::CanService::Request>();
    request->can_id = CAN_ID;
    request->write_data.push_back(DATA[0]);

    auto future = can_client_->async_send_request(request);
    auto response = future.get();

    EXPECT_EQ(response->success, false);
}
