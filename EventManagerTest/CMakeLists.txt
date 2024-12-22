cmake_minimum_required(VERSION 3.16)

project(EventManagerTest LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(EventManagerTest tst_eventmanagertest.cpp)
add_test(NAME EventManagerTest COMMAND EventManagerTest)

target_link_libraries(EventManagerTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)

target_sources(EventManagerTest PRIVATE
    ../cluster_qt/includes/ArrowSymbolWidget.h
    ../cluster_qt/includes/SpeedometerWidget.h
    ../cluster_qt/includes/Blinkers.h
    ../cluster_qt/includes/EventManager.h
    ../cluster_qt/ArrowSymbolWidget.cpp
    ../cluster_qt/SpeedometerWidget.cpp
    ../cluster_qt/Blinkers.cpp
    ../cluster_qt/EventManager.cpp
)
