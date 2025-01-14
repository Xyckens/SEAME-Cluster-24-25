#include "INA219Driver.hpp"
#include "INA219def.hpp"
#include <fmt/core.h>

using namespace std::chrono_literals;

INA219Driver::INA219Driver(std::shared_ptr<rclcpp::Node> node,
                           uint8_t device_address)
    : ADriver(node, device_address), current_divider_mA_(0),
      power_multiplier_mW_(0), calibration_value_(0)
{
    publisher_voltage_ =
        node_->create_publisher<std_msgs::msg::Float64>("battery_voltage", 10);
    publisher_perc_ = node_->create_publisher<std_msgs::msg::Float64>(
        "battery_percentage", 10);

    while (!i2c_client_->wait_for_service(2s))
        RCLCPP_INFO(node_->get_logger(), "Waiting for i2c service to start");

    this->ping();

    RCLCPP_INFO(node->get_logger(),
                "INA219 succefully initiated at address: 0x%02X",
                device_address_);
}

/**
 * @brief set callibration value according to the range of voltage expected.
 *
 * To see more about how the value is obtained, refer to:
 * https://github.com/adafruit/Adafruit_INA219/blob/master/Adafruit_INA219.cpp
 */
void INA219Driver::setCalibration_32V_1A()
{
    calibration_value_ = 10240;

    // Set multipliers to convert raw current / power
    current_divider_mA_ = 25;
    power_multiplier_mW_ = 0.8f;

    // Set calibration register
    writeRegister(INA219_REG_CALIBRATION, calibration_value_);

    // Set Config register to take into account the settings above
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
                      INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
                      INA219_CONFIG_SADCRES_12BIT_1S_532US |
                      INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

    writeRegister(INA219_REG_CONFIG, config);
}

/**
 * @brief Write a 16-bit value to a specified register.
 *
 * The 16-bit value is split into two 8-bit values because the I2C service
 * processes data as arrays of bytes.
 *
 * @param reg Register address to write to.
 * @param value 16-bit value to write to the register.
 */
void INA219Driver::writeRegister(uint8_t reg, uint16_t value)
{
    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__device_address(device_address_);
    request->set__read_request(false);
    request->write_data.push_back(reg);
    request->write_data.push_back((value >> 4) & 0xFF);
    request->write_data.push_back(value & 0xFF);

    i2c_client_->async_send_request(request,
                                    std::bind(&INA219Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
}

/**
 * @brief Read data from a specific register of the INA219.
 *
 * This function sends an asynchronous I2C read request to the device.
 * The response is handled via a callback. A "callback registry" pattern
 * is used, where callbacks associated with specific registers are stored
 * in a map. Upon receiving the response, the corresponding callback is
 * invoked to process the data.
 *
 * @param reg Register address to read from.
 * @param length Number of bytes to read.
 * @param callback Function to call with the retrieved data once the read
 * operation completes.
 */

void INA219Driver::readRegister(uint8_t reg, uint8_t length,
                                ReadCallback callback)
{
    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__device_address(device_address_);
    request->set__read_request(true);
    request->set__read_length(length);
    request->write_data.push_back(reg);

    // store the callback
    read_callbacks_[reg] = callback;

    i2c_client_->async_send_request(
        request,
        [this, reg](
            rclcpp::Client<custom_msgs::srv::I2cService>::SharedFuture response)
        { this->handleI2cReadResponse(response, reg); });
}

/**
 * @brief Handle the asynchronous response of a read request.
 *
 * This function retrieves the callback associated with the register that was
 * read, using the callback registry. If a callback is found, it is invoked
 * with the retrieved data and removed.
 *
 * @param response Future object containing the result of the I2C read request.
 * @param reg The register address that was read, used to find the associated
 * callback.
 */
void INA219Driver::handleI2cReadResponse(
    rclcpp::Client<custom_msgs::srv::I2cService>::SharedFuture response,
    uint8_t reg)
{
    auto result = response.get();

    if (!result->success)
    {
        RCLCPP_ERROR(node_->get_logger(), "I2C read fail for register 0x%02X",
                     reg);
        return;
    }

    // Retrieve the callback for this register
    auto callback_iter = read_callbacks_.find(reg);
    if (callback_iter != read_callbacks_.end())
    {
        auto callback = callback_iter->second;
        callback(result->read_data); // Invoke the callback with the data
        read_callbacks_.erase(callback_iter); // Clean up
    }
    else
    {
        RCLCPP_WARN(node_->get_logger(),
                    "No callback registered for register 0x%02X", reg);
    }
}

void INA219Driver::publishBusVoltage()
{
    readRegister(INA219_REG_BUSVOLTAGE, 2,
                 [this](std::vector<uint8_t> data)
                 {
                     if (data.size() >= 2)
                     {
                         uint16_t raw_value = (data[0] << 8) | data[1];
                         float bus_voltage =
                             (raw_value >> 3) * 0.004; // Convert to volts
                         RCLCPP_DEBUG(rclcpp::get_logger("INA219"),
                                      "Bus Voltage: %.2f V", bus_voltage);

                         auto msg = std_msgs::msg::Float64();
                         msg.set__data(bus_voltage);
                         this->publisher_voltage_->publish(msg);

                         uint8_t battery_level =
                             (bus_voltage - 10.8) / (12.6 - 10.8) * 100;
                         auto msg_perc = std_msgs::msg::Float64();
                         msg_perc.set__data(battery_level);
                         this->publisher_perc_->publish(msg_perc);
                     }
                     else
                     {
                         RCLCPP_ERROR(rclcpp::get_logger("INA219"),
                                      "Failed to read Bus Voltage");
                     }
                 });
}
