#include "I2cInterface.hpp"
#include "I2cDriver.hpp"
#include <fcntl.h>
#include <fmt/core.h>
#include <string>

/**
 * @brief Construct a new I2c Interface object. throw an exception if the i2c
 * driver fails to initiate.
 *
 * @param mock_driver if not null, the mock object will be used
 * @param service_name "i2c_service" by default
 */
I2cInterface::I2cInterface(std::shared_ptr<II2cDriver> mock_driver,
                           const std::string& service_name)
    : Node("i2c_interface")
{
    if (mock_driver)
        i2c_driver_ = mock_driver;
    else
        i2c_driver_ = std::make_shared<I2cDriver>(1);

    rclcpp::QoS qos((rclcpp::KeepLast(QUEUE_SIZE)));
    qos.reliable();
    qos.durability_volatile();

    i2c_service_ = this->create_service<custom_msgs::srv::I2cService>(
        service_name,
        std::bind(&I2cInterface::handleI2cRequest, this, std::placeholders::_1,
                  std::placeholders::_2),
        qos.get_rmw_qos_profile());
    init();

    RCLCPP_INFO(this->get_logger(), "Starting i2c interface");
}

/**
 * @brief Open the /dev/i2c.
 *
 * this operation will fail if the user is not in the i2c group ("sudo usermod
 * -aG i2c $USER")
 */
void I2cInterface::init()
{
    int res;

    res = i2c_driver_->open();
    if (res < 0)
    {
        RCLCPP_ERROR(this->get_logger(), "Fail opening /dev/i2c-%ud",
                     i2c_driver_->getDeviceNb());
        throw std::runtime_error("Could not open i2c device");
    }
}

/**
 * @brief Function beeing called upon receiving a request to the i2c service.
 *
 * the struct of the i2c service allows writting and reading operations. To see
 * the service definition do "ros2 interface show custom_msgs/srv/I2cService" or
 * see JetRacer/src/bus_interfaces/custom_msgs/srv/I2cService.
 *
 * @param request the request object containing read / write / both request
 * @param response the response object to be filled with the data requested and
 * success mesage to communicate to the client
 */
void I2cInterface::handleI2cRequest(
    const std::shared_ptr<custom_msgs::srv::I2cService::Request> request,
    std::shared_ptr<custom_msgs::srv::I2cService::Response> response)
{
    response->set__success(true);

    if (i2c_driver_->setAddress(request->device_address) != 0)
    {
        std::string error_msg =
            fmt::format("Fail to set the device at address 0x{:02X}",
                        request->device_address);
        RCLCPP_ERROR(this->get_logger(), "%s", error_msg.c_str());
        response->set__success(false);
        response->set__message(error_msg);
        return;
    }

    ssize_t write_res = i2c_driver_->write(request->write_data);
    if (write_res != request->write_data.size())
    {
        std::string error_msg;
        if (write_res < 0)
            error_msg = fmt::format("Fail to write to device 0x{:02X}",
                                    request->device_address);
        else
            error_msg = fmt::format("Incomplete write from device 0x{:02X}",
                                    request->device_address);
        RCLCPP_ERROR(this->get_logger(), "%s", error_msg.c_str());
        response->set__success(false);
        response->set__message(error_msg);
        return;
    }
    RCLCPP_DEBUG(this->get_logger(), "Succefully writen at address 0x%02X",
                 request->device_address);

    if (request->read_request)
    {
        std::vector<uint8_t> data(request->read_length);
        int res = i2c_driver_->read(data);
        if (res != request->read_length)
        {
            std::string error_msg;
            if (res < 0)
                error_msg = fmt::format("Fail to read from device 0x{:02X}",
                                        request->device_address);
            else
                error_msg = fmt::format("Incomplete read from device 0x{:02X}. "
                                        "expected {:d} got {:d}",
                                        request->device_address,
                                        request->read_length, res);
            RCLCPP_ERROR(this->get_logger(), "%s", error_msg.c_str());
            response->set__success(false);
            response->set__message(error_msg);
            return;
        }
        response->set__read_data(data);
    }
}
