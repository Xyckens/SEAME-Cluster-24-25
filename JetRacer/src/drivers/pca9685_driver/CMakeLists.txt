cmake_minimum_required(VERSION 3.8)
project(pca9685_driver)

# Export compile command for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set library name
set(lib_name PCA9685Driver)

# Find dependencies first
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msgs REQUIRED)
find_package(abstract_driver REQUIRED)

# Add source files
set(srcs
  src/PCA9685Driver.cpp
)

# Create the shared library
add_library(${lib_name} SHARED ${srcs})

# Set include directories for the library
target_include_directories(${lib_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Gtest
if(BUILD_TESTING)
  find_package(ament_cmake_gmock REQUIRED)
  ament_add_gmock(${PROJECT_NAME}_test test/src/PCA9685DriverTest.cpp test/src/MockI2cService.cpp)
  target_include_directories(${PROJECT_NAME}_test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    test/include
)
  target_link_libraries(${PROJECT_NAME}_test ${lib_name})
endif()

# Install
install(
  TARGETS ${lib_name}
  EXPORT  ${lib_name}_targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# Link against the required dependencies
ament_target_dependencies(${lib_name} 
  rclcpp 
  custom_msgs 
  abstract_driver
)

install(TARGETS ${lib_name} DESTINATION lib)
install(
  DIRECTORY include/
  DESTINATION include
)

ament_export_targets(${lib_name}_targets HAS_LIBRARY_TARGET)
ament_export_dependencies(${dependencies})
ament_package()
