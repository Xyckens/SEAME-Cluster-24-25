cmake_minimum_required(VERSION 3.8)

project(can_interface)

# Enable compile commands export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define executable name
set(exec_name ${PROJECT_NAME}_node)

# Add source files
set(srcs
  src/main.cpp
  src/CanInterface.cpp
  src/RCanDriver.cpp
  include/ICanDriver.hpp
)

# Include necessary directories
include_directories(
  include
)

# Find dependencies and custom message
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msgs REQUIRED)

# Fetch external library for CAN driver
include(FetchContent)
FetchContent_Declare(
  sockcanpp
  GIT_REPOSITORY https://github.com/SimonCahill/libsockcanpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(sockcanpp)


# Add the executable
add_executable(${exec_name} ${srcs})
ament_target_dependencies(${exec_name} rclcpp custom_msgs)

# Link external libraries
target_link_libraries(${exec_name} sockcanpp)

# Gtest
if(BUILD_TESTING)
  find_package(ament_cmake_gmock REQUIRED)
  
  # Create test executable
  ament_add_gmock(${PROJECT_NAME}_test 
	  test/src/CanInterfaceTest.cpp
	  test/include/MockCanDriver.hpp
	  src/CanInterface.cpp
	  src/RCanDriver.cpp
	  include/ICanDriver.hpp
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
  )
  target_link_libraries(${PROJECT_NAME}_test sockcanpp)
endif()

# Install target
install(TARGETS ${exec_name} DESTINATION lib/${PROJECT_NAME})

# Export dependencies
ament_package()

