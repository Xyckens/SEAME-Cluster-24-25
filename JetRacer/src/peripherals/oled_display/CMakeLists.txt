cmake_minimum_required(VERSION 3.8)

project(oled_display)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(exec_name ${PROJECT_NAME}_node)

set(srcs 
	src/main.cpp
	src/OledDisplayNode.cpp)

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(ssd1306_driver REQUIRED)

#specify the full path of where th lib should be 
link_directories(/home/jetpack/SEAME-Cluster-24-25/JetRacer/install/ssd1306_driver/lib)

# Include directories for the ssd1306 and linux_i2c sources
include_directories(
  include
  ${CMAKE_SOURCE_DIR}/include
  ~/SEAME-Cluster-24-25/JetRacer/install/ssd1306_driver/include
  ~/SEAME-Cluster-24-25/JetRacer/install/abstract_driver/include
)

# Add the executable and link necessary libraries
add_executable(${exec_name} ${srcs})

target_link_libraries(${exec_name} SSD1306Driver)  # Link to PCA9685Driver

ament_target_dependencies(${exec_name} rclcpp std_msgs custom_msgs ssd1306_driver)

# Install target
install(TARGETS ${exec_name} DESTINATION lib/${PROJECT_NAME})

ament_package()
