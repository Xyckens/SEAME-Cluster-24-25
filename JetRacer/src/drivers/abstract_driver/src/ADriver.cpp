#include "ADriver.hpp"
#include "fmt/core.h"

using namespace std::chrono_literals;

ADriver::ADriver(rclcpp::Node::SharedPtr node, uint8_t device_address)
    : node_(node), device_address_(device_address)
{
    i2c_client_ =
        node_->create_client<custom_msgs::srv::I2cService>("i2c_service");

    while (!i2c_client_->wait_for_service(1s))
        RCLCPP_INFO(node->get_logger(), "Waiting for i2c service to start");
}

ADriver::~ADriver() {}

/**
 * @brief handle the response from the i2c service
 *
 * @param response
 */
void ADriver::handleI2cResponse(
    rclcpp::Client<custom_msgs::srv::I2cService>::SharedFuture future)
{
    auto response = future.get();
    if (!response->success)
        RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
}

/**
 * @brief ping the device and wait for a response
 *
 * This method uses spin_until_future_complete() which will spin the node while
 * waiting for the response. This function can then not be called after the node
 * has been spinned by the main executor.
 *
 * @return
 */
void ADriver::ping()
{
    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__device_address(device_address_);
    request->set__read_request(false);
    request->write_data.push_back(0x0);

    auto response_future = i2c_client_->async_send_request(request);

    // Spin until the future is complete
    if (rclcpp::spin_until_future_complete(node_, response_future, 5s) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
        auto response = response_future.get();

        // Check if the response was successful
        if (!response->success)
        {
            std::string msg =
                fmt::format("Device 0x{:02X} not found", device_address_);
            throw DriverException(msg);
        }
    }
    else
    {
        std::string msg = fmt::format("request for device 0x{:02X} timed out",
                                      device_address_);
        throw DriverException(msg);
    }
}
