cmake_minimum_required(VERSION 3.8)

project(i2c_interface)

# Enable compile commands export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define executable name
set(exec_name ${PROJECT_NAME}_node)

# Add source files
set(srcs
  src/main.cpp
  src/I2cInterface.cpp
  src/I2cDriver.cpp
  include/II2cDriver.hpp
)

# Include necessary directories
include_directories(
  include
)

# Find dependencies and custom message
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msgs REQUIRED)
find_package(fmt REQUIRED)

# Add the executable
add_executable(${exec_name} ${srcs})
ament_target_dependencies(${exec_name} rclcpp custom_msgs fmt)
target_link_libraries(${exec_name} fmt::fmt)

# Gtest
if(BUILD_TESTING)
  find_package(ament_cmake_gmock REQUIRED)
  
  # Create test executable
  ament_add_gmock(${PROJECT_NAME}_test 
    test/src/I2cInterfaceTest.cpp
	test/include/MockI2cDriver.hpp
	include/II2cDriver.hpp
	src/I2cInterface.cpp
	src/I2cDriver.cpp
  )
  
  target_include_directories(${PROJECT_NAME}_test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    test/include
  )
  
  # Add dependencies to the test target
  ament_target_dependencies(${PROJECT_NAME}_test 
    rclcpp 
    custom_msgs
    fmt
  )
  target_link_libraries(${PROJECT_NAME}_test fmt::fmt)
endif()


# Install target
install(TARGETS ${exec_name} DESTINATION lib/${PROJECT_NAME})

# Export dependencies
ament_package()

