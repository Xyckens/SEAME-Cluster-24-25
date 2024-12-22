cmake_minimum_required(VERSION 3.16)

project(_cluster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

set(PROJECT_SOURCES
        mainwindow.ui

        includes/mainwindow.h
        includes/ArrowSymbolWidget.h
        includes/SpeedometerWidget.h
        includes/CPUTempWidget.h
        includes/FanSpeedWidget.h
        includes/BatteryWidget.h
        includes/TopBar.h
        includes/Blinkers.h
        includes/EventManager.h
        includes/CameraWidget.h
        includes/BatteryAndSpeedWidget.h
        includes/Cluster.h
        
        mainwindow.cpp
        ArrowSymbolWidget.cpp
        SpeedometerWidget.cpp
        CPUTempWidget.cpp
        FanSpeedWidget.cpp
        BatteryWidget.cpp
        TopBar.cpp
        Blinkers.cpp
        EventManager.cpp
        CameraWidget.cpp
        BatteryAndSpeedWidget.cpp

        main.cpp
)

qt_add_executable(_cluster
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(_cluster PRIVATE Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets)

set_target_properties(_cluster PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS _cluster
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(_cluster)
endif()
