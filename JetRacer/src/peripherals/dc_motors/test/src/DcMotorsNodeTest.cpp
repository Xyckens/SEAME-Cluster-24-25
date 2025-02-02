#include "DcMotorsNodeTest.hpp"
#include "PCA9685Driver.hpp"

#define VEL_TO_PW(x) static_cast<float>(x) * (MAX_COUNT - MIN_COUNT); // NOLINT

using namespace testing;

void DcMotorsNodeTest::SetUp()
{
    rclcpp::init(0, nullptr);
    mock_driver_ = std::make_shared<MockPCA9685Driver>();
    test_node_ = std::make_shared<DcMotorsNode>();
    test_node_->initPCA9685(mock_driver_);

    cmd_vel_pub_ = test_node_->create_publisher<geometry_msgs::msg::Twist>(
        "cmd_vel", NODE_QOS);
    executor_ = std::make_unique<rclcpp::executors::SingleThreadedExecutor>();
    executor_->add_node(test_node_);
    executor_thread_ = std::thread([this]() { executor_->spin(); });
}

void DcMotorsNodeTest::TearDown()
{
    executor_->cancel();
    if (executor_thread_.joinable())
        executor_thread_.join();
    rclcpp::shutdown();
}

INSTANTIATE_TEST_CASE_P(linearVelocities, DcMotorsNodeTest,
                        Values(1, -1, 0.5, -0.5, 0.25, -0.25)); // NOLINT

TEST_P(DcMotorsNodeTest, ValidSpeed)
{
    const auto linear_vel = GetParam();
    const uint16_t expected_pw = VEL_TO_PW(std::abs(linear_vel));
    InSequence seq;

    EXPECT_CALL(*mock_driver_,
                setPWMDutyCycle(DEFAULT_CHANNEL, 1, expected_pw));
    EXPECT_CALL(*mock_driver_, setPWMDutyCycle(7, 1, expected_pw));

    // Expect GPIO calls based on direction
    if (linear_vel > 0)
    { // Forward
        EXPECT_CALL(*mock_driver_, setGPIO(1, false));
        EXPECT_CALL(*mock_driver_, setGPIO(2, true));
        EXPECT_CALL(*mock_driver_, setGPIO(6, false));
        EXPECT_CALL(*mock_driver_, setGPIO(5, true));
    }
    else if (linear_vel < 0)
    { // Reverse
        EXPECT_CALL(*mock_driver_, setGPIO(1, true));
        EXPECT_CALL(*mock_driver_, setGPIO(2, false));
        EXPECT_CALL(*mock_driver_, setGPIO(6, true));
        EXPECT_CALL(*mock_driver_, setGPIO(5, false));
    }
    else
    { // Stop
        EXPECT_CALL(*mock_driver_, setGPIO(1, false));
        EXPECT_CALL(*mock_driver_, setGPIO(2, false));
        EXPECT_CALL(*mock_driver_, setGPIO(6, false));
        EXPECT_CALL(*mock_driver_, setGPIO(5, false));
    }

    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = linear_vel;

    cmd_vel_pub_->publish(msg);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
