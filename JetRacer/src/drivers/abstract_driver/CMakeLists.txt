cmake_minimum_required(VERSION 3.8)

project(abstract_driver)

# Export compile command for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set library name
set(lib_name AbstractDriver)

include_directories(
  include
)

# Add source files
set(srcs
	src/ADriver.cpp
)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msgs REQUIRED)
find_package(fmt REQUIRED)

# Create the shared library
add_library(${lib_name} SHARED ${srcs})

# Link against the required dependencies
ament_target_dependencies(${lib_name} rclcpp custom_msgs fmt)

# Export the libraries
install(TARGETS ${lib_name} DESTINATION lib)
install(
  DIRECTORY include/
  DESTINATION include
)

ament_package()
