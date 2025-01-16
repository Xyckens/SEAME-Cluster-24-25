#include "../includes/EventManager.h"
#include <QSinglePointEvent>
#include <QtMath>

EventManager::EventManager(ArrowSymbolWidget* arrow,
                           SpeedometerWidget* py_speed, Blinkers* left_blinker,
                           Blinkers* right_blinker,
                           QStackedWidget* stackedWidget,
                           std::shared_ptr<RosNode> ros_node)
    : arrows(arrow), py_speed(py_speed), left_blinker(left_blinker),
      right_blinker(right_blinker), stackedWidget(stackedWidget), node(ros_node)
{
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this,
            &EventManager::processKeyStates);
    updateTimer->start(80); // Every 80ms
    executor.add_node(node);
}

bool EventManager::eventFilter(QObject* obj, QEvent* event)
{
    // Handle Mouse Press Button
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mousePos = static_cast<QMouseEvent*>(event);
        QPointF pressPosition = mousePos->position();

        mousePosition = pressPosition;
        return true;
    }
    // Handle Mouse Button Release
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mousePos = static_cast<QMouseEvent*>(event);
        QPointF releasePosition = mousePos->position();
        // Check for swipe params. If enough conditions, returns true for swipe.
        if (swipe(releasePosition))
            return true;
    }
    // Handle key events
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        pressedKeys.insert(keyEvent->key());
        return true;
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        pressedKeys.remove(keyEvent->key());
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void EventManager::processKeyStates()
{
    // TODO: spin the node. uodates the meber variable of RosNode

    executor.spin_some(
        std::chrono::milliseconds(10)); // after this line, values are updated.
    py_speed->setCurrentSpeed(node->getSpeed());
    py_speed->update();
    RCLCPP_INFO(node->get_logger(), "current_speed: %d", node->getSpeed());

    //
    // for (int key : pressedKeys)
    // {
    //     switch (key)
    //     {
    //     case Qt::Key_Left:
    //     case Qt::Key_Right:
    //     case Qt::Key_Up:
    //         arrows->changeDirection(key);
    //         break;
    //     case Qt::Key_Space:
    //     case Qt::Key_Down:
    //         py_speed->accelerate(key);
    //         break;
    //     case Qt::Key_L:
    //         left_blinker->turnOnBlinkers(key);
    //         break;
    //     case Qt::Key_R:
    //         right_blinker->turnOnBlinkers(key);
    //         break;
    //     default:
    //         break;
    //     }
    // }
}

bool EventManager::swipe(QPointF releasePosition)
{
    bool swipeRange = qFabs(mousePosition.rx() - releasePosition.rx()) > 100;
    bool verticalRange = qFabs(mousePosition.ry() - releasePosition.ry()) < 200;

    if (mousePosition.rx() < releasePosition.rx() && swipeRange &&
        verticalRange)
    {
        int prevIndex = std::max(0, stackedWidget->currentIndex() - 1);
        stackedWidget->setCurrentIndex(prevIndex);

        return true;
    }
    else if (mousePosition.rx() > releasePosition.rx() && swipeRange &&
             verticalRange)
    {
        int nextIndex = std::min(stackedWidget->count() - 1,
                                 stackedWidget->currentIndex() + 1);
        stackedWidget->setCurrentIndex(nextIndex);

        return true;
    }
    return false;
}

QStackedWidget* EventManager::getStackedWidget(void) const
{
    return stackedWidget;
}
