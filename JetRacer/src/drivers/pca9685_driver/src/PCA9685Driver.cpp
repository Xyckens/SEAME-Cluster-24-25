#include "PCA9685Driver.hpp"

using namespace std::chrono_literals;

PCA9685Driver::PCA9685Driver(std::shared_ptr<rclcpp::Node> node,
                             uint8_t device_address)
    : ADriver(node, device_address)
{
    this->ping();

    RCLCPP_INFO(node->get_logger(),
                "PCA9685 succefully initiated at address: 0x%02X",
                device_address_);
}

/**
 * @brief Writes a value to a specific register on the PCA9685 device.
 *
 * This function sends a write request to the I2C bus, specifying the target
 * register and the value to be written. It uses an asynchronous service call
 * to handle the I2C communication.
 *
 * @param reg The register address to write to.
 * @param value The value to write to the register.
 */
void PCA9685Driver::setRegister(uint8_t reg, uint8_t value)
{
    auto request = std::make_shared<custom_msgs::srv::I2cService::Request>();

    request->set__read_request(false);
    request->set__device_address(device_address_);
    request->write_data.push_back(reg);
    request->write_data.push_back(value);

    i2c_client_->async_send_request(request,
                                    std::bind(&PCA9685Driver::handleI2cResponse,
                                              this, std::placeholders::_1));
}

/**
 * @brief Sets the PWM frequency of the PCA9685 device.
 *
 * The PWM frequency determines the speed at which PWM pulses are generated.
 * This function calculates the prescale value based on the desired frequency
 * and sets it on the device. The prescale value is derived from the formula:
 *   prescale = round((25MHz / (4096 * freq_Hz)) - 1)
 *
 * @param freq_Hz The desired PWM frequency in hertz.
 */
void PCA9685Driver::setPWMFrequency(float freq_hz)
{
    auto prescale =
        static_cast<uint8_t>((CRISTAL_FREQUENCY / (NB_STEPS * freq_hz)) - 1.0);

    setRegister(MODE1_REGISTER, SLEEP_MODE); // sleep mode
    setRegister(PRESCALE_REGISTER, prescale);
    setRegister(MODE1_REGISTER, AUTO_INCREMENT); // Wake-up + auto increment
    std::this_thread::sleep_for(5ms);
}

/**
 * @brief Sets the duty cycle for a specified PWM channel.
 *
 * The PCA9685 uses a 12-bit resolution (0-4095) to encode the duty cycle.
 * The "on" and "off" parameters specify the start and end of the PWM signal
 * within this 12-bit range. For example:
 * - on = 0, off = 2048 sets a 50% duty cycle:
 *   duty cycle (%) = ((off - on) / 4096) * 100 = 50%
 *
 * Since the 12-bit values must be written across two 8-bit registers,
 * the values are split into:
 * - Low byte (first 8 bits)
 * - High byte (last 4 bits)
 *
 * @param channel The PWM channel to control (0-15).
 * @param on The 12-bit start time of the signal (0-4095).
 * @param off The 12-bit end time of the signal (0-4095).
 */
void PCA9685Driver::setPWMDutyCycle(uint8_t channel, uint16_t on, uint16_t off)
{
    if (channel > MAX_CHANNEL)
    {
        RCLCPP_ERROR(node_->get_logger(), "Invalid chanel number: %d", channel);
        return;
    }

    auto low_on = (uint8_t)(on & LSB_MASK);
    auto high_on = (uint8_t)((on >> 8) & LSB_MASK); // NOLINT
    auto low_off = (uint8_t)(off & LSB_MASK);
    auto high_off = (uint8_t)((off >> 8) & LSB_MASK); // NOLINT

    setRegister((uint8_t)(CHANNEL_REGISTER + (channel * 4)), low_on);
    setRegister((uint8_t)(CHANNEL_REGISTER + (channel * 4) + 1), high_on);
    setRegister((uint8_t)(CHANNEL_REGISTER + (channel * 4) + 2), low_off);
    setRegister((uint8_t)(CHANNEL_REGISTER + (channel * 4) + 3), high_off);
}

/**
 * @brief set a gpio of the PCA9685 high or low
 *
 * Set ON to 0x1000 and OFF to 0x0000 to make the output HIGH.
 * Set ON to 0x0000 and OFF to 0x1000 to make the output LOW.
 *
 * @param channel The PWM channel to control (0-15).
 * @param on true for high, false for low.
 */
void PCA9685Driver::setGPIO(uint8_t channel, bool on)
{
    setPWMDutyCycle(channel, on * 0x1000, !on * 0x1000); // NOLINT
}
