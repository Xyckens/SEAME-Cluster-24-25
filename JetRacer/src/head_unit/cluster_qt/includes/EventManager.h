#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "ArrowSymbolWidget.h"
#include "BatteryAndSpeedWidget.h"
#include "BatteryWidget.h"
#include "Blinkers.h"
#include "CPUTempWidget.h"
#include "Colors.h"
#include "FanSpeedWidget.h"
#include "RosNode.hpp"
#include "SpeedometerWidget.h"
#include "StatsWidget.h"
#include "ListeningWidget.h"
#include "TopBar.h"

#include <QObject>
#include <QSet>
#include <QStackedWidget>
#include <QSwipeGesture>
#include <QTimer>
#include <QWidget>
#include <rclcpp/rclcpp.hpp>


/**
 * @brief The EventManager handles the events sent by the car and the command.
 * @details It performs a swipe movement based on mouse events, filter key events to change
 *  the cluster color and its units (KH/Mh).
 */

class EventManager : public QWidget
{
        Q_OBJECT;

    private:
        ArrowSymbolWidget* arrows;
        SpeedometerWidget* py_speed;
        BatteryWidget* py_battery;
        BatteryAndSpeedWidget* py_batspeed;
        Blinkers* left_blinker;
        Blinkers* right_blinker;
        Blinkers* left_blinker2;
        Blinkers* right_blinker2;
        StatsWidget* stats;
        FanSpeedWidget* fan;
        FanSpeedWidget* fan2;
        CPUTempWidget* cpu;
        CPUTempWidget* cpu2;
        TopBar* top;
        TopBar* top2;
        ListeningWidget* listening;

        QPointF                                     mouse_position;
        QTimer*                                     update_timer; // Used to check if a key is still being called
        QStackedWidget*                             stacked_widget;
        QWidget*                                    main_window;
        std::shared_ptr<RosNode>                    node;
        rclcpp::executors::SingleThreadedExecutor   executor;

        static constexpr int    REFRESH_RATE_MS = 50;
        static constexpr int    SCREEN_UPDATE_RATE = 10;
        static constexpr int    STATS_DISTANCE = 4533;
        static constexpr int    STATS_AVERAGE = 56;
        static constexpr int    STATS_CONSUMED = 78;
        static constexpr int    STATS_OBSTACLES = 2;
        static constexpr int    MIN_SWIPE_WIDTH = 100;
        static constexpr int    MAX_SWIPE_HEIGHT = 200;

    public:
        EventManager(ArrowSymbolWidget* arrow, SpeedometerWidget* py_speed,
                     BatteryWidget* py_battery,
                     BatteryAndSpeedWidget* py_batspeed, Blinkers* left_blinker,
                     Blinkers* right_blinker, Blinkers* left_blinker2,
                     Blinkers* right_blinker2, StatsWidget* stats,
                     FanSpeedWidget* fan, FanSpeedWidget* fan2,
                     CPUTempWidget* cpu, CPUTempWidget* cpu2, TopBar* top,
                     TopBar* top2, QStackedWidget* stacked_widget,
                     QWidget* main_window, ListeningWidget* listening,
                     std::shared_ptr<RosNode> ros_node);
        ~EventManager() override;
        QStackedWidget* getStackedWidget();
        void            changeColors();
        void            changeUnits();
        Color           color1;

    protected:
        bool    eventFilter(QObject* obj, QEvent* event) override;
        bool    swipe(QPointF release_position);
        void    activateButton(int key);  


    private slots:
        void updateScreen();
        void updateBlinkers();
        void updateArrows();
};

#endif
