## Overview

The JetRacer project is organized into a modular structure, where each package represents a ROS2 node encapsulating a specific aspect of the system. The architecture is divided into three main layers, each with its own responsibility:

- **Bus Interfaces**: Directly interact with hardware via different protocols (CAN, I2C).
- **Peripherals**: Communicate with the drivers and manage higher-level device functionality.
- **Head Unit**: Coordinates the peripherals and handles higher-level logic.

```
.
├── bus_interfaces
│   ├── can_interface
│   └── i2c_interface
├── drivers
│   ├── abstract_driver
│   ├── custom_msgs
│   ├── ina219_driver
│   ├── pca9685_driver
│   └── ssd1306_driver
├── head_unit
│   ├── display_routine
│   └── teleop
├── peripherals
│   ├── battery
│   ├── camera
│   ├── dc_motors
│   ├── oled_display
│   ├── servo_motor
│   └── speed_sensor
└── tests
    └── i2c_peripheral_stress_test
```
## Layered Architecture

The project structure follows a clear layered architecture that helps isolate responsibilities and ensures modularity. The three main layers are:

### 1. **Bus Interfaces (can_interface, i2c_interface)**
These packages manage the direct interaction with hardware data buses. They are the closest to the hardware and abstract the communication with physical devices such as CAN and I2C interfaces.
- **Responsibility**: Communicate with hardware via data buses (CAN, I2C).
- **ROS2 Mechanism**: Provide services or topics for peripheral nodes to interact with hardware.

### 2. **Drivers (abstract_driver, pca9685_driver, etc.)**
Drivers encapsulate device-specific logic into reusable objects. Each driver handles the low-level details of interacting with its associated hardware. These drivers are now decoupled from the peripheral nodes to promote code reuse and maintainability.
- **Responsibility**: Provide device-specific functionality, abstracting away low-level hardware interactions.
- **Design Improvement**: Peripheral nodes now hold an instance of their corresponding driver. If the driver fails to initialize (e.g., due to hardware issues), the peripheral node will not start, preventing undefined behavior or unnecessary crashes.
- **ROS2 Mechanism**: Exposed as libraries to be used by peripherals or other nodes.

### 3. **Peripherals (oled_display, servo_motor, dc_motors, etc.)**
Peripherals interact with drivers to manage the higher-level functionality of each device. For example, the `servo_motor` package uses the `pca9685_driver` to control servos. Each peripheral node initializes its driver and interact with the bus interfaces via his driver instance.
- **Responsibility**: Manage higher-level device functionality and interact with drivers.
- **Design Improvement**: Peripheral nodes depend on the proper initialization of their corresponding driver. If the driver cannot initialize successfully (e.g., due to a missing or malfunctioning device), the peripheral node will fail gracefully.

### 4. **Head Unit (coordinator, infotainment)**
The head unit represents the higher-level logic of the system, coordinating the operation of peripherals and providing user interaction functionality (e.g., infotainment system, control coordination).
- **Responsibility**: Coordinate and manage the operation of peripherals.
- **ROS2 Mechanism**: Provide high-level functionalities and user interactions through nodes that communicate with peripheral nodes.


## Modularity and Separation of Concerns

This structure ensures that:

- **Bus Interfaces**: Abstraction of hardware communication for flexibility and scalability.
- **Drivers**: Encapsulation of device-specific logic into reusable libraries, simplifying peripheral implementation and promoting code reuse.
- **Peripherals**: High-level logic tied to devices, ensuring clear dependency on drivers and bus interfaces.
- **Head Unit**: Higher-level coordination and user interaction, ensuring easy maintenance and extendability.

By maintaining a clear separation of concerns, the architecture enhances system flexibility, modularity, and ease of maintenance.

## Build Instructions

To build the JetRacer project, follow these steps:

1. Open a terminal and navigate to the root directory of the project:
    ```bash
    cd JetRacer
    ```

2. Build the project using `colcon`:
    ```bash
    colcon build --symlink-install
    ```

    This will compile the source code and set up the appropriate environment for running the nodes.

3. Execute the launch script:
    ```bash
    ros2 launch launch/JetRacer_launch.py
    ```

4. Open a terminal to launch the `rqt` console and start interacting with topics, or directly publish into topics:

    To launch the `rqt` console:
    ```bash
    rqt
    ```

    Or directly publish a topic:
    ```bash
    ros2 topic list
    ros2 topic pub cmd_display custom_msgs/Display '{line1: "Hello world"}' --once
    ```
