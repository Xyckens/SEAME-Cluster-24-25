cmake_minimum_required(VERSION 3.16)
project(cluster VERSION 0.1 LANGUAGES CXX)

# Define executable name
set(exec_name ${PROJECT_NAME}_node)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt configuration
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 dependencies
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Test)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Test)

# Find ROS dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(custom_msgs REQUIRED)

# Define source and header files separately
set(HEADERS
    includes/mainwindow.h
    includes/ArrowSymbolWidget.h
    includes/SpeedometerWidget.h
    includes/CPUTempWidget.h
    includes/FanSpeedWidget.h
    includes/BatteryWidget.h
    includes/BatteryAndSpeedWidget.h
    includes/TopBar.h
    includes/Blinkers.h
    includes/EventManager.h
    includes/CameraWidget.h
	includes/RosNode.hpp
	includes/Colors.h
    #includes/SettingsWidget.h
    includes/StatsWidget.h
    includes/ListeningWidget.h
)

set(SOURCES
    sources/mainwindow.cpp
    sources/ArrowSymbolWidget.cpp
    sources/SpeedometerWidget.cpp
    sources/CPUTempWidget.cpp
    sources/FanSpeedWidget.cpp
    sources/BatteryWidget.cpp
    sources/BatteryAndSpeedWidget.cpp
    sources/TopBar.cpp
    sources/Blinkers.cpp
    sources/EventManager.cpp
    sources/CameraWidget.cpp
	sources/RosNode.cpp
	#sources/SettingsWidget.cpp
	sources/StatsWidget.cpp
    sources/ListeningWidget.cpp
)

set(UI_FILES
    sources/mainwindow.ui
)

# Create a shared library for tests
add_library(${PROJECT_NAME}_lib SHARED
    ${HEADERS}
    ${SOURCES}
    ${UI_FILES}
)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${rclcpp_INCLUDE_DIRS}
    ${std_msgs_INCLUDE_DIRS}
    ${custom_msgs_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ${rclcpp_LIBRARIES}
    ${std_msgs_LIBRARIES}
    ${custom_msgs_LIBRARIES}
)

# Main executable using qt_add_executable
qt_add_executable(${exec_name}
    MANUAL_FINALIZATION
    sources/main.cpp
    ${HEADERS}
    ${SOURCES}
    ${UI_FILES}
)

target_include_directories(${exec_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${rclcpp_INCLUDE_DIRS}
    ${std_msgs_INCLUDE_DIRS}
    ${custom_msgs_INCLUDE_DIRS}
)

target_link_libraries(${exec_name} PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ${rclcpp_LIBRARIES}
    ${std_msgs_LIBRARIES}
    ${custom_msgs_LIBRARIES}
)

# Testing configuration
if(BUILD_TESTING)
    enable_testing()

    function(add_qt_test TEST_NAME TEST_SOURCES)
        qt_add_executable(${TEST_NAME} ${TEST_SOURCES})
        target_link_libraries(${TEST_NAME} PRIVATE
            ${PROJECT_NAME}_lib
            Qt6::Test
        )
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endfunction()

    file(GLOB_RECURSE TEST_FILES "test/*Test/*.cpp")
    foreach(TEST_FILE ${TEST_FILES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        add_qt_test(${TEST_NAME} ${TEST_FILE})
    endforeach()
 
endif()

# Install targets
install(TARGETS ${exec_name}
    RUNTIME DESTINATION lib/${PROJECT_NAME}
)

# Export package dependencies
ament_package()

# Finalize Qt6 executable
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${exec_name})
endif()
