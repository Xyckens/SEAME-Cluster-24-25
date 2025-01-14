# **SSD1306 Driver**

## **Code Structure**

### **Classes and Key Components**

1. **`SSD1306Driver`**  
   The core driver class responsible for interfacing with the SSD1306 OLED display via I2C. It abstracts the display's initialization, configuration, and rendering functionalities.  

   - **Key Methods:**
     - `initDisplay()`: Initializes the display by sending test read and write operations.
     - `onOffDisplay(uint8_t onoff)`: Turns the display on or off based on the provided parameter.
     - `setDefaultConfig()`: Configures the display with default settings, including resolution, contrast, and memory mode.
     - `setCursor(uint8_t x, uint8_t y)`: Sets the cursor position for writing data to the display.
 Flips the display horizontally based on the parameter.
     - `rotateDisplay(uint8_t degree)`: Rotates the display by 0 or 180 degrees.
     - `clearRow(uint8_t row)`: Clears a specific row of the display.
     - `clearScreen()`: Clears the entire display by iterating through all rows.
     - `writeString(uint8_t size, const std::string& msg)`: Writes a string of characters to the display using the specified font size.

2. **ROS2 Interface**  
   - **Service Client**:  
     Utilizes the `custom_msgs::srv::I2cService` to send I2C commands for controlling the SSD1306 display.  
   - **Service Communication**:  
   he I2C service.

3. **Display Configuration and Control**
   - **Initialization and Configuration**:  
     The `setDefaultConfig()` method ensures that the display is initialized with standard settings, such as turning on the charge pump, setting contrast, and enabling page mode.  
   - **Display Modes**:  
     - Turn the display on/off.
     - Flip or rotate the display.  
   - **Cursor and Drawing**:  
     - Positioning is managed using `setCursor()`.
     - Data for rendering characters or clearing rows is sent via I2C commands.

4. **Font Handling**  
   - Provides support for two font sizes: small (`font5x7`) and normal (`font8x8`).
   - Each character's pixel data is retrieved from the font table and written to the display.

### **Key Constants and Registers**
- **Display Address**:  
  `SSD1306_I2C_ADDR` is the I2C address of the SSD1306 display.
- **Control Bytes**:  
  - `SSD1306_COMM_CONTROL_BYTE`: Control byte for commands.
  - `SSD1306_DATA_CONTROL_BYTE`: Control byte for data.  
- **Configuration Commands**:  
  Includes commands for setting contrast, enabling the charge pump, configuring memory mode, and controlling display orientation.  
- **Dimensions**:  
  - `SSD1306_WIDTH`: Display width.
  - `SSD1306_HEIGHT`: Display height.

---

This structure ensures flexibility in interacting with the SSD1306 display while adhering to the ROS2 framework for communication and control.
  Asynchronous communication with the I2C service is handled using `async_send_request()`. The `handleI2cResponse()` processes the responses from t    - `flipDisplay(uint8_t flip)`: 
