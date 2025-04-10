#include "../includes/EventManager.h"
#include <rclcpp/rclcpp.hpp>


/**
 * @brief Constructs an EventManager with the specified widgets and ROS node.
 *
 * This constructor initializes the EventManager, connects it to various widgets 
 * and sets up a timer to periodically update the screen. It also initializes
 * the ROS node and starts the update timer.
 *
 * @param arrow Pointer to the ArrowSymbolWidget.
 * @param py_speed Pointer to the SpeedometerWidget.
 * @param py_battery Pointer to the BatteryWidget.
 * @param py_batspeed Pointer to the BatteryAndSpeedWidget.
 * @param left_blinker Pointer to the left Blinkers widget.
 * @param right_blinker Pointer to the right Blinkers widget.
 * @param left_blinker2 Pointer to the secondary left Blinkers widget.
 * @param right_blinker2 Pointer to the secondary right Blinkers widget.
 * @param stats Pointer to the StatsWidget.
 * @param fan Pointer to the FanSpeedWidget.
 * @param fan2 Pointer to the secondary FanSpeedWidget.
 * @param cpu Pointer to the CPUTempWidget.
 * @param cpu2 Pointer to the secondary CPUTempWidget.
 * @param top Pointer to the TopBar widget.
 * @param top2 Pointer to the secondary TopBar widget.
 * @param stacked_widget Pointer to the QStackedWidget for managing multiple screens.
 * @param main_window Pointer to the main QWidget window.
 * @param ros_node Shared pointer to the ROS node for communication.
 */
EventManager::EventManager(
    ArrowSymbolWidget* arrow, SpeedometerWidget* py_speed,
    BatteryWidget* py_battery, BatteryAndSpeedWidget* py_batspeed,
    Blinkers* left_blinker, Blinkers* right_blinker, Blinkers* left_blinker2,
    Blinkers* right_blinker2, StatsWidget* stats, FanSpeedWidget* fan,
    FanSpeedWidget* fan2, CPUTempWidget* cpu, CPUTempWidget* cpu2, TopBar* top,
    TopBar* top2, QStackedWidget* stacked_widget, QWidget* main_window,
    ListeningWidget* listening, std::shared_ptr<RosNode> ros_node)
    : arrows(arrow), py_speed(py_speed), py_battery(py_battery),
      py_batspeed(py_batspeed), left_blinker(left_blinker),
      right_blinker(right_blinker), left_blinker2(left_blinker2),
      right_blinker2(right_blinker2), stats(stats), fan(fan), fan2(fan2),
      cpu(cpu), cpu2(cpu2), top(top), top2(top2), stacked_widget(stacked_widget),
      main_window(main_window), listening(listening), node(ros_node)
{
    color1 = Color();
    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &EventManager::updateScreen);
    executor.add_node(node);
    update_timer->start(REFRESH_RATE_MS); // Every 50ms => 20FPS
}

/**
 * @brief Filters and handles events for the EventManager.
 *
 * This method processes mouse events, and key events. It tracks mouse
 * positions for swipe gestures and handles key presses to activate buttons.
 *
 * @param obj The object that generated the event.
 * @param event The event to be filtered.
 * @return true if the event was handled, false otherwise.
 */
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

/**
 * @brief Activates functionality based on the key pressed.
 *
 * This method handles key press events and performs actions such as changing colors or units.
 *
 * @param key The key code of the pressed key.
 */
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

/**
 * @brief Updates the screen with the latest data from the ROS node.
 *
 * This method is called periodically by the update timer. It retrieves the latest speed, battery
 * level, and blinker state from the ROS node and updates the corresponding widgets.
 */
void EventManager::updateScreen()
{
    executor.spin_some(
        std::chrono::milliseconds(SCREEN_UPDATE_RATE)); // after this line, values are updated.
    py_speed->setCurrentSpeed(node->getSpeed());
    py_battery->setCurrentLevel(node->getBattery());
    py_batspeed->setCurrentLevel(node->getBattery());
    py_batspeed->setCurrentSpeed(node->getSpeed());
    //arrows->changeDirection(node->getWheelAngle(), "forward");
    
    updateBlinkers();
    updateArrows();
    stats->setDistance(STATS_DISTANCE);
    stats->setAverage(STATS_AVERAGE);
    stats->setConsumed(STATS_CONSUMED);
    stats->setObstacles(STATS_OBSTACLES);
    stats->update();
}

/**
 * @brief Updates the state of the blinker widgets based on the ROS node's blinker state.
 *
 * This method turns the blinkers on or off depending on the current blinker state.
 */
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

/**
 * @brief Updates the state of the arrow widgets based on the ROS node's arrow state.
 *
 * This method changes the arrows color depending on the current arrow state.
 */
void EventManager::updateArrows()
{
    switch (node->getLaneDetection())
    {
    case laneDetection::NONE:
        arrows->turnOnLanes(false, false);
        break;
    case laneDetection::RIGHT:
        arrows->turnOnLanes(false, true);
        break;
    case laneDetection::LEFT:
        arrows->turnOnLanes(true, false);
        break;
    default:
        arrows->turnOnLanes(false, false);
    }
}

/**
 * @brief Changes the color scheme of all connected widgets.
 *
 * This method updates the color scheme of all widgets and the main 
 * window's background based on the current color index.
 */
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
    //listening->changeColor(color1.counter);
    main_window->setStyleSheet(color1.background_array[color1.counter]);
}

/**
 * @brief Changes the units of measurement for speed and temperature widgets.
 *
 * This method toggles between different units for the speedometer and CPU temperature widgets.
 */
void EventManager::changeUnits()
{
    py_speed->changeUnits();
    py_batspeed->changeUnits();
    stats->changeUnits();
    cpu->changeUnits();
    cpu2->changeUnits();
}

/**
 * @brief Handles swipe gestures to switch between screens.
 *
 * This method detects horizontal swipe gestures and updates the QStackedWidget to show the
 * previous or next screen.
 *
 * @param release_position The position where the mouse button was released.
 * @return true if a swipe gesture was detected, false otherwise.
 */
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

/**
 * @brief A Getter that returns the QStackedWidget used for managing multiple screens.
 *
 * @return A pointer to the QStackedWidget.
 */
QStackedWidget* EventManager::getStackedWidget() { return stacked_widget; }

/**
 * @brief Destroys the EventManager.
 *
 * This destructor cleans up any resources used by the EventManager.
 */
EventManager::~EventManager() {}
