cmake_minimum_required(VERSION 3.16)

project(cluster_tests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ClusterShared
    ../include/ArrowSymbolWidget.h
    ../include/SpeedometerWidget.h
    ../include/Blinkers.h
    ../include/EventManager.h
    ../include/TopBar.h
    ../include/BatteryAndSpeedWidget.h

    ../src/ArrowSymbolWidget.cpp
    ../src/SpeedometerWidget.cpp
    ../src/Blinkers.cpp
    ../src/EventManager.cpp
    ../src/TopBar.cpp
    ../src/BatteryAndSpeedWidget.cpp
)

target_link_libraries(ClusterShared PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

function(SETUP_TESTS)
    foreach(test_folder ${ARGN})
        file(GLOB test_files "${test_folder}/*.cpp") # Store the .cpp files into test_files.
        foreach(test_file ${test_files})
            get_filename_component(_testname ${test_file} NAME_WE) # Get the filename without the extension (WE).
            add_executable(${_testname} ${test_file})
            add_test(NAME ${_testname} COMMAND ${_testname})
            target_link_libraries(${_testname} PRIVATE ClusterShared Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
        endforeach()
    endforeach()
endfunction()

SETUP_TESTS(
    EventManagerTest
    BlinkerTest
    TopBarTest
    SpeedometerTest
)
