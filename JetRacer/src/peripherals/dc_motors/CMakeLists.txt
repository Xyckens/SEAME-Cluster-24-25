cmake_minimum_required(VERSION 3.8)

project(dc_motors)

# Export compile command for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set executable name
set(exec_name ${PROJECT_NAME}_node)

# Add source files
set(srcs
    src/main.cpp
	src/DcMotorsNode.cpp
)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pca9685_driver REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(custom_msgs REQUIRED)

#specify the full path of where the lib should be 
link_directories(/home/jetpack/SEAME-Cluster-24-25/JetRacer/install/pca9685_driver/lib)

# Include directories
include_directories(
  include
  ${CMAKE_SOURCE_DIR}/include
  ~/SEAME-Cluster-24-25/JetRacer/install/pca9685_driver/include
  ~/SEAME-Cluster-24-25/JetRacer/install/abstract_driver/include
)

# Add the executable and link necessary libraries
add_executable(${exec_name} ${srcs})

# Link the PCA9685Driver library to the executable
target_link_libraries(${exec_name} PCA9685Driver)  # Link to PCA9685Driver

ament_target_dependencies(${exec_name} rclcpp pca9685_driver geometry_msgs custom_msgs)

# Install target
install(TARGETS ${exec_name} DESTINATION lib/${PROJECT_NAME})

ament_package()

