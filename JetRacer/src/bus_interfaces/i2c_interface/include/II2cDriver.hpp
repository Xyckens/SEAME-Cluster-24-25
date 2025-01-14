#pragma once

#include <cstdint>
#include <rclcpp/rclcpp.hpp>

#pragma once
#include <cstdint>
#include <rclcpp/rclcpp.hpp>

/**
 * @class AI2cDevice
 * @brief Abstract interface for I2C device operations
 *
 * This abstract class provides a hardware abstraction layer for I2C
 * communication, allowing for dependency injection and easier unit testing. By
 * implementing this interface, concrete classes can provide either real
 * hardware communication or mock implementations for testing.
 *
 * The interface wraps the essential I2C operations (open, read, write, and
 * address setting) into virtual methods that can be overridden by concrete
 * implementations.
 */
class II2cDriver
{
    public:
        /**
         * @brief Constructs an I2C device interface
         * @param device The I2C bus number (e.g., 1 for /dev/i2c-1)
         */
        II2cDriver(uint8_t device) : device_(device){};
        virtual ~II2cDriver() = default;

        /**
         * @brief Opens the I2C device
         * @return 0 on success, negative value on error
         */
        virtual int open() = 0;

        /**
         * @brief Reads data from the I2C device
         * @param buff Vector to store read data
         * @return Number of bytes read, or negative value on error
         */
        virtual ssize_t read(std::vector<uint8_t>& buff) = 0;

        /**
         * @brief Writes data to the I2C device
         * @param buff Vector containing data to write
         * @return Number of bytes written, or negative value on error
         */
        virtual ssize_t write(std::vector<uint8_t>& buff) = 0;

        /**
         * @brief Sets the I2C slave device address
         * @param address 7-bit I2C device address
         * @return 0 on success, negative value on error
         */
        virtual int setAddress(uint8_t address) = 0;

        /**
         * @brief Gets the I2C bus number
         * @return The bus number used by this device
         */
        uint8_t getDeviceNb() const { return device_; }

    protected:
        uint8_t device_; ///< I2C bus number
        int i2c_fd_;     ///< File descriptor for the I2C device
};
