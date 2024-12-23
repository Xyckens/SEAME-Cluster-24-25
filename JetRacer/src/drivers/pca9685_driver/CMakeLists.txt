cmake_minimum_required(VERSION 3.8)

project(pca9685_driver)

# Export compile command for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set library name
set(lib_name PCA9685Driver)

include_directories(
  include
  ~/SEAME-Cluster-24-25/JetRacer/install/abstract_driver/include
)

link_directories(/home/jetpack/SEAME-Cluster-24-25/JetRacer/install/abstract_driver/lib)

# Add source files
set(srcs
  src/PCA9685Driver.cpp
)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msgs REQUIRED)
find_package(abstract_driver REQUIRED)

# Create the shared library
add_library(${lib_name} SHARED ${srcs})

# Link against the required dependencies
ament_target_dependencies(${lib_name} rclcpp custom_msgs abstract_driver)

target_link_libraries(${lib_name} AbstractDriver)

# Export the libraries
install(TARGETS ${lib_name} DESTINATION lib)
install(
  DIRECTORY include/
  DESTINATION include
)

ament_package()
