#ifndef CLUSTER_H
# define CLUSTER_H 

# include <QApplication>
# include <QHBoxLayout>
# include <QLabel>
# include <QKeyEvent>
# include <cmath>

# ifdef __has_include
#  if __has_include(<rclcpp/rclcpp.hpp>)
#   include <rclcpp/rclcpp.hpp>
#  else
#   warning "rclcpp/rclcpp.hpp not found"
#  endif
# endif

// # include <rclcpp/rclcpp.hpp>

# include "ArrowSymbolWidget.h"
# include "SpeedometerWidget.h"
# include "FanSpeedWidget.h"
# include "CPUTempWidget.h"
# include "BatteryWidget.h"
# include "BatteryAndSpeedWidget.h"
# include "TopBar.h"
# include "Blinkers.h"
# include "EventManager.h"
# include "CameraWidget.h"

# include <QStackedWidget>
# include <QToolBar>
# include <QGestureEvent>
# include <QSwipeGesture>

#endif
