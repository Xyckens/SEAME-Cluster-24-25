# **PCA9685 Driver**

## **Code Structure**

### **Classes and Key Components**

1. **`PCA9685Driver`**
   The core driver class responsible for interacting with the PCA9685 PWM driver via I2C. It abstracts the functionality to configure the device, control PWM channels, and handle I2C communication.

   - **Key Methods:**
     - `setRegister()`: Writes a value to a specific register of the PCA9685.
     - `handleI2cResponse()`: Handles the response from the I2C service after sending a request.
     - `setPWMFrequency()`: Configures the PCA9685 PWM frequency by adjusting the prescaler register.
     - `setPWMDutyCycle()`: Sets the duty cycle for a specific PWM channel.
     - `setGPIO()`: Sets a GPIO pin of the PCA9685 high or low.

2. **ROS2 Interface**
   - **Service Client**:
     The `custom_msgs::srv::I2cService` is used to send I2C commands to the PCA9685 driver, including writing values to specific registers.
   - **Service Communication**:
     The `async_send_request()` method is used to send requests to the I2C service asynchronously, while the `handleI2cResponse()` method processes the responses.

3. **PWM Configuration**
   - **Prescaler Calculation**: The PWM frequency is set by configuring the prescaler register based on the desired frequency.
   - **Duty Cycle Control**: Each PWM channel has its "on" and "off" times (12-bit resolution). These are set through multiple 8-bit register writes.

### **Key Constants and Registers**
- `MODE1_REGISTER`: The register controlling the main operational mode of the PCA9685.
- `PRESCALE_REGISTER`: The register used to set the frequency of PWM output.
- `CHANNEL_REGISTER`: The base register for controlling individual PWM channels.

---
