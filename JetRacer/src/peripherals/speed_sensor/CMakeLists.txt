cmake_minimum_required(VERSION 3.8)

project(speed_sensor)

# Export comompilecommand for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set executable name
set(exec_name ${PROJECT_NAME}_node)

include_directories(
  include
  ${CMAKE_SOURCE_DIR}/include
)

# ADD source files
set(srcs 
	src/main.cpp
	src/SpeedSensorNode.cpp)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msgs REQUIRED)
find_package(std_msgs REQUIRED)


# Add the executable and link necessary libraries
add_executable(${exec_name} ${srcs})

ament_target_dependencies(${exec_name} rclcpp custom_msgs std_msgs)

# Install target
install(TARGETS ${exec_name} DESTINATION lib/${PROJECT_NAME})

ament_export_dependencies(rclcpp std_msgs custom_msgs)
ament_package()

