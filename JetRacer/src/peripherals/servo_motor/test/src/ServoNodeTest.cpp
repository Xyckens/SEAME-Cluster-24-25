#include "ServoNodeTest.hpp"
#include "PCA9685Driver.hpp"

constexpr float angleToPWM(int x)
{
    return MIN_COUNT +
           (static_cast<float>(x) * (MAX_COUNT - MIN_COUNT)) / 180; // NOLINT
}

void ServoNodeTest::SetUp()
{
    rclcpp::init(0, nullptr);

    mock_driver_ = std::make_shared<MockPCA9685Driver>();
    test_node_ = std::make_shared<ServoNode>();
    test_node_->initPCA9685(mock_driver_);
    cmd_vel_pub_ = test_node_->create_publisher<geometry_msgs::msg::Twist>(
        "cmd_vel", NODE_QOS);
    executor_ = std::make_unique<rclcpp::executors::SingleThreadedExecutor>();
    executor_->add_node(test_node_);

    executor_thread_ = std::thread([this]() { executor_->spin(); });
}

void ServoNodeTest::TearDown()
{
    executor_->cancel();
    if (executor_thread_.joinable())
        executor_thread_.join();
    rclcpp::shutdown();
}

/**
 * @brief check if the cmd_vel is properly converted into a pulse width
 */
TEST_P(ServoNodeTest, velocityToAngleConversion)
{
    const auto [cmd_vel, expected_angle] = GetParam();
    const uint16_t expected_pw = angleToPWM(expected_angle);

    EXPECT_CALL(*mock_driver_, setPWMDutyCycle)
        .WillOnce(
            [expected_pw](uint8_t channel, uint16_t on, uint16_t off)
            {
                EXPECT_EQ(channel, DEFAULT_CHANNEL);
                EXPECT_EQ(on, 0);
                EXPECT_EQ(off, expected_pw);
            });

    auto message = geometry_msgs::msg::Twist();
    message.angular.z = cmd_vel;
    cmd_vel_pub_->publish(message);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// NOLINTBEGIN
//  Define test cases: {cmd_vel, expected_angle}
INSTANTIATE_TEST_CASE_P(
    VelocityMappings, ServoNodeTest,
    ::testing::Values(std::make_tuple(-1.0, 170), // Full left
                      std::make_tuple(0.0, 90),   // Center
                      std::make_tuple(1.0, 10),   // Full right
                      std::make_tuple(-0.5, 135), // Mid left
                      std::make_tuple(0.5, 45)    // Mid right
                      ),
    [](const testing::TestParamInfo<ServoNodeTest::ParamType>& info)
    {
        double cmd_vel = std::get<0>(info.param);
        uint8_t angle = std::get<1>(info.param);
        std::stringstream ss;
        ss << "CmdVel_" << (cmd_vel < 0 ? "n" : "")
           << static_cast<int>(std::abs(cmd_vel * 10)) << "_Angle_"
           << static_cast<int>(angle);
        return ss.str();
    });
// NOLINTEND
