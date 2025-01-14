#include "MockI2cService.hpp"

MockI2cServiceNode::MockI2cServiceNode() : rclcpp::Node("mock_i2c_service")
{
    i2c_service_ = create_service<custom_msgs::srv::I2cService>(
        "i2c_service",
        [this](
            const std::shared_ptr<custom_msgs::srv::I2cService_Request> request,
            const std::shared_ptr<custom_msgs::srv::I2cService_Response>
                response)
        { mock_service_.HandleRequest(*request, *response); });
}

MockI2cService& MockI2cServiceNode::getMockService() { return mock_service_; }
