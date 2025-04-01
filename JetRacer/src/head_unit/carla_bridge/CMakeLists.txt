cmake_minimum_required(VERSION 3.8)

project(carla_bridge)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(exec_name ${PROJECT_NAME}_node)

set(srcs
  src/main.cpp
  src/CarlaBridgeNode.cpp
)

include_directories(
  include
)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(carla_msgs REQUIRED)

add_executable(${exec_name} ${srcs})

ament_target_dependencies(${exec_name} 
    rclcpp
    geometry_msgs
    carla_msgs
)

install(TARGETS
    ${exec_name} 
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()
