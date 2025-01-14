cmake_minimum_required(VERSION 3.8)

project(ssd1306_driver)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set library name
set(lib_name SSD1306Driver)

include_directories(
  include
  ~/SEAME-Cluster-24-25/JetRacer/install/abstract_driver/include
)

link_directories(/home/jetpack/SEAME-Cluster-24-25/JetRacer/install/abstract_driver/lib)

set(srcs 
	src/SSD1306Driver.cpp)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(abstract_driver REQUIRED)

# Create the shared library
add_library(${lib_name} SHARED ${srcs})


ament_target_dependencies(${lib_name} rclcpp std_msgs custom_msgs abstract_driver)

target_link_libraries(${lib_name} AbstractDriver)

# Export the libraries
install(TARGETS ${lib_name} DESTINATION lib)
install(
  DIRECTORY include/
  DESTINATION include

)

ament_package()
