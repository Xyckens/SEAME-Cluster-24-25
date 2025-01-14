#include <custom_msgs/srv/i2c_service.hpp>
#include <gmock/gmock.h>
#include <rclcpp/rclcpp.hpp>

/**
 * @class MockI2cService
 * @brief mocks the request handling.
 *
 * The idea is to intercept the request and examine its content to see if the
 * driver works as expected. To do so we will spin a node exposing the i2c
 * service but the callback associated with the service will just call the mock
 * service that we can control the behaviour of.
 *
 */
class MockI2cService
{
    public:
        MOCK_METHOD(void, HandleRequest,
                    (const custom_msgs::srv::I2cService_Request&,
                     custom_msgs::srv::I2cService_Response&));
};

class MockI2cServiceNode : public rclcpp::Node
{
    public:
        MockI2cServiceNode();
        MockI2cService& getMockService();

    private:
        rclcpp::Service<custom_msgs::srv::I2cService>::SharedPtr i2c_service_;
        MockI2cService mock_service_;
};
