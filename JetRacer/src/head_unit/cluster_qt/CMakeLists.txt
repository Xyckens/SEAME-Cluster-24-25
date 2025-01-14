cmake_minimum_required(VERSION 3.16)
project(cluster VERSION 0.1 LANGUAGES CXX)

# Define executable name
set(exec_name ${PROJECT_NAME}_node)

# Qt configuration
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 dependencies
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# Find ROS dependencies
#find_package(ament_cmake REQUIRED)
#find_package(rclcpp REQUIRED)
#find_package(std_msgs REQUIRED)
#find_package(custom_msgs REQUIRED)

# Define source and header files separately
set(HEADERS
    include/mainwindow.h
    include/ArrowSymbolWidget.h
    include/SpeedometerWidget.h
    include/CPUTempWidget.h
    include/FanSpeedWidget.h
    include/BatteryWidget.h
    include/BatteryAndSpeedWidget.h
    include/TopBar.h
    include/Blinkers.h
    include/EventManager.h
    include/CameraWidget.h
)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/ArrowSymbolWidget.cpp
    src/SpeedometerWidget.cpp
    src/CPUTempWidget.cpp
    src/FanSpeedWidget.cpp
    src/BatteryWidget.cpp
    src/BatteryAndSpeedWidget.cpp
    src/TopBar.cpp
    src/Blinkers.cpp
    src/EventManager.cpp
    src/CameraWidget.cpp
)

set(UI_FILES
    src/mainwindow.ui
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${rclcpp_INCLUDE_DIRS}
    ${std_msgs_INCLUDE_DIRS}
    ${custom_msgs_INCLUDE_DIRS}
)

# Add executable with all files
qt_add_executable(${exec_name}
    MANUAL_FINALIZATION
    ${HEADERS}
    ${SOURCES}
    ${UI_FILES}
)

# Link libraries
target_link_libraries(${exec_name} PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ${rclcpp_LIBRARIES}
    ${std_msgs_LIBRARIES}
    ${custom_msgs_LIBRARIES}
)

# Explicitly set include directories for the target
target_include_directories(${exec_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${rclcpp_INCLUDE_DIRS}
    ${std_msgs_INCLUDE_DIRS}
    ${custom_msgs_INCLUDE_DIRS}
)

# Install target
install(TARGETS ${exec_name}
    RUNTIME DESTINATION lib/${PROJECT_NAME}
)

# Export package dependencies
#ament_package()

# Finalize Qt6 executable
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${exec_name})
endif()
