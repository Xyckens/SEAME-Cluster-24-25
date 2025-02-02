#include "../includes/EventManager.h"
#include <rclcpp/rclcpp.hpp>

EventManager::EventManager(
    ArrowSymbolWidget* arrow, SpeedometerWidget* py_speed,
    BatteryWidget* py_battery, BatteryAndSpeedWidget* py_batspeed,
    Blinkers* left_blinker, Blinkers* right_blinker, Blinkers* left_blinker2,
    Blinkers* right_blinker2, StatsWidget* stats, FanSpeedWidget* fan,
    FanSpeedWidget* fan2, CPUTempWidget* cpu, CPUTempWidget* cpu2, TopBar* top,
    TopBar* top2, QStackedWidget* stacked_widget, QWidget* main_window,
    std::shared_ptr<RosNode> ros_node)
    : arrows(arrow), py_speed(py_speed), py_battery(py_battery),
      py_batspeed(py_batspeed), left_blinker(left_blinker),
      right_blinker(right_blinker), left_blinker2(left_blinker2),
      right_blinker2(right_blinker2), stats(stats), fan(fan), fan2(fan2),
      cpu(cpu), cpu2(cpu2), top(top), top2(top2), stacked_widget(stacked_widget),
      main_window(main_window), node(ros_node)
{
    color1 = Color();
    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &EventManager::updateScreen);
    executor.add_node(node);
    update_timer->start(REFRESH_RATE_MS); // Every 50ms => 20FPS
}

bool EventManager::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        auto mouse_event = static_cast<QMouseEvent*>(event);
        QPointF press_position = mouse_event->position();

        mouse_position = press_position;
        return true;
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        auto mouse_event = static_cast<QMouseEvent*>(event);
        QPointF release_position = mouse_event->position();
        if (swipe(release_position))
            return true;
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        auto key_event = static_cast<QKeyEvent*>(event);
        
        if (!key_event->isAutoRepeat())
            activateButton(key_event->key());
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void    EventManager::activateButton(int key)
{
    switch (key)
    {
    case Qt::Key_C:
        changeColors();
        break;
    case Qt::Key_U:
        changeUnits();
        break;
    default:
        break;
    }
}

void EventManager::updateScreen()
{
    executor.spin_some(
        std::chrono::milliseconds(SCREEN_UPDATE_RATE)); // after this line, values are updated.
    py_speed->setCurrentSpeed(node->getSpeed());
    py_battery->setCurrentLevel(node->getBattery());
    py_batspeed->setCurrentLevel(node->getBattery());
    py_batspeed->setCurrentSpeed(node->getSpeed());
    updateBlinkers();

    stats->setDistance(STATS_DISTANCE);
    stats->setAverage(STATS_AVERAGE);
    stats->setConsumed(STATS_CONSUMED);
    stats->setObstacles(STATS_OBSTACLES);
    stats->update();
}

void EventManager::updateBlinkers()
{
    switch (node->getBlinkerState())
    {
    case blinkerState::IDLE:
        left_blinker->turnOnBlinkers(false);
        right_blinker->turnOnBlinkers(false);
        left_blinker2->turnOnBlinkers(false);
        right_blinker2->turnOnBlinkers(false);
        break;
    case blinkerState::TURN_LEFT:
        left_blinker->turnOnBlinkers(true);
        right_blinker->turnOnBlinkers(false);
        left_blinker2->turnOnBlinkers(true);
        right_blinker2->turnOnBlinkers(false);
        break;
    case blinkerState::TURN_RIGHT:
        left_blinker->turnOnBlinkers(false);
        right_blinker->turnOnBlinkers(true);
        left_blinker2->turnOnBlinkers(false);
        right_blinker2->turnOnBlinkers(true);
        break;
    case blinkerState::WARNINGS:
        left_blinker->turnOnBlinkers(true);
        right_blinker->turnOnBlinkers(true);
        left_blinker2->turnOnBlinkers(true);
        right_blinker2->turnOnBlinkers(true);
        break;
    default:
        left_blinker->turnOnBlinkers(false);
        right_blinker->turnOnBlinkers(false);
        left_blinker2->turnOnBlinkers(false);
        right_blinker2->turnOnBlinkers(false);
    }
}

void EventManager::changeColors()
{
    color1.indent();
    arrows->changeColor(color1.counter);
    py_batspeed->changeColor(color1.counter);
    py_speed->changeColor(color1.counter);
    py_battery->changeColor(color1.counter);
    stats->changeColor(color1.counter);
    top->changeColor(color1.counter);
    left_blinker->changeColor(color1.counter);
    right_blinker->changeColor(color1.counter);
    top2->changeColor(color1.counter);
    left_blinker2->changeColor(color1.counter);
    right_blinker2->changeColor(color1.counter);
    fan->changeColor(color1.counter);
    fan2->changeColor(color1.counter);
    cpu->changeColor(color1.counter);
    cpu2->changeColor(color1.counter);
    main_window->setStyleSheet(color1.background_array[color1.counter]);
}

void EventManager::changeUnits()
{
    py_speed->changeUnits();
    py_batspeed->changeUnits();
    stats->changeUnits();
    cpu->changeUnits();
    cpu2->changeUnits();
}

bool    EventManager::swipe(QPointF release_position)
{
    bool swipe_range = qFabs(mouse_position.rx() - release_position.rx()) > MIN_SWIPE_WIDTH;
    bool vertical_range = qFabs(mouse_position.ry() - release_position.ry()) < MAX_SWIPE_HEIGHT;

    if (mouse_position.rx() < release_position.rx() && swipe_range && vertical_range ) {
        int previous_index = std::max(0, stacked_widget->currentIndex() - 1);
        stacked_widget->setCurrentIndex(previous_index);

        return true;
    }
    else if (mouse_position.rx() > release_position.rx() && swipe_range && vertical_range ) {
        int next_index = std::min(stacked_widget->count() - 1, stacked_widget->currentIndex() + 1);
        stacked_widget->setCurrentIndex(next_index);

        return true;
    }
    return false;
}


QStackedWidget* EventManager::getStackedWidget() { return stacked_widget; }

EventManager::~EventManager() {}
