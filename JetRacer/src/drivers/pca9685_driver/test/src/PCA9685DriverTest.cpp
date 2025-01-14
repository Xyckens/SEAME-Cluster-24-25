#include "PCA9685DriverTest.hpp"
#include "PCA9685Driver.hpp"

using ::testing::InSequence;

void PCA9685DriverTest::SetUp()
{
    rclcpp::init(0, nullptr);
    test_node_ = std::make_shared<rclcpp::Node>("test_node");
    mock_i2c_node_ = std::make_shared<MockI2cServiceNode>();

    executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    executor_->add_node(mock_i2c_node_);
    executor_thread_ = std::thread([this]() { executor_->spin(); });
}

void PCA9685DriverTest::TearDown()
{
    executor_->cancel();
    if (executor_thread_.joinable())
        executor_thread_.join();
    rclcpp::shutdown();
}

TEST_F(PCA9685DriverTest, TestInstantiationSuccess)
{
    // The ping() call in the constructor should request a write of 0x0 at
    // device_address
    EXPECT_CALL(mock_i2c_node_->getMockService(), HandleRequest)
        .WillOnce(
            [](const auto& request, auto& response)
            {
                EXPECT_EQ(request.device_address, 0x60);
                EXPECT_EQ(request.write_data[0], 0x00);
                response.success = true;
            });

    PCA9685Driver driver(test_node_, 0x60);
}

TEST_F(PCA9685DriverTest, TestInstantiationFailure)
{
    EXPECT_CALL(mock_i2c_node_->getMockService(), HandleRequest)
        .WillOnce([](const auto& request, auto& response)
                  { response.success = false; });

    EXPECT_THROW({ PCA9685Driver driver(test_node_, 0x60); }, DriverException);
}

TEST_F(PCA9685DriverTest, TestSetPWMFrequency)
{
    InSequence seq;
    float frequency = 50.0;

    // First expect the ping() call from constructor
    EXPECT_CALL(mock_i2c_node_->getMockService(), HandleRequest)
        .WillOnce(
            [](const auto& req, auto& resp)
            {
                EXPECT_EQ(req.device_address, 0x60);
                EXPECT_EQ(req.write_data[0], 0x00);
                resp.success = true;
            });

    // // Then expect the setPWMFrequency sequence
    EXPECT_CALL(mock_i2c_node_->getMockService(), HandleRequest)
        .WillOnce(
            [](const auto& req, auto& resp)
            {
                EXPECT_EQ(req.device_address, 0x60);
                EXPECT_EQ(req.write_data[0], MODE1_REGISTER);
                EXPECT_EQ(req.write_data[1], 0x10); // Sleep mode
                resp.success = true;
            });

    EXPECT_CALL(mock_i2c_node_->getMockService(), HandleRequest)
        .WillOnce(
            [frequency](const auto& req, auto& resp)
            {
                EXPECT_EQ(req.device_address, 0x60);
                EXPECT_EQ(req.write_data[0], PRESCALE_REGISTER);
                // Calculate expected prescale for 50Hz
                uint8_t expected_prescale = static_cast<uint8_t>(
                    (25000000.0 / (4096 * frequency)) - 1.0);
                EXPECT_EQ(req.write_data[1], expected_prescale);
                resp.success = true;
            });

    EXPECT_CALL(mock_i2c_node_->getMockService(), HandleRequest)
        .WillOnce(
            [](const auto& req, auto& resp)
            {
                EXPECT_EQ(req.device_address, 0x60);
                EXPECT_EQ(req.write_data[0], MODE1_REGISTER);
                EXPECT_EQ(req.write_data[1], 0x80); // Wake-up + auto
                resp.success = true;
            });

    PCA9685Driver driver(test_node_, 0x60);
    driver.setPWMFrequency(frequency);
}
