#include <BatteryNode.hpp>
#include <rclcpp/client.hpp>
#include <rclcpp/logger.hpp>
#include <rclcpp/logging.hpp>

using namespace std::chrono_literals;

BatteryNode::BatteryNode() : Node("battery_node")
{
    timer_ = this->create_wall_timer(
        5s, std::bind(&BatteryNode::publishBatteryLevel, this));
}

/**
 * @brief Initializes the INA219 driver.
 *
 * @return EXIT_FAILURE if the driver fails to initialize, EXIT_SUCCESS
 * otherwise.
 */
uint8_t BatteryNode::initINA219()
{
    try
    {
        ina219_driver_ = std::make_shared<INA219Driver>(
            this->shared_from_this(), INA219_ADDRESS);
    }
    catch (const std::exception& e)
    {
        RCLCPP_ERROR(this->get_logger(), "%s", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Publishes the battery level.
 */
void BatteryNode::publishBatteryLevel() { ina219_driver_->publishBusVoltage(); }
