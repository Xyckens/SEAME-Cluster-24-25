cmake_minimum_required(VERSION 3.8)

project(teleop)

# Enable compile commands export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define executable name
set(exec_name ${PROJECT_NAME}_node)

# Add source files
set(srcs
  src/main.cpp
  src/TeleopNode.cpp
)

# Include necessary directories
include_directories(
  include
)

# Find dependencies and custom message
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)


# Add the executable
add_executable(${exec_name} ${srcs})
ament_target_dependencies(${exec_name} rclcpp geometry_msgs sensor_msgs)

# Install target
install(TARGETS ${exec_name} DESTINATION lib/${PROJECT_NAME})

# Export dependencies
ament_package()
