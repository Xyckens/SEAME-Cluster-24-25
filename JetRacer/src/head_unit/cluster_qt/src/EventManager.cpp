#include <QSinglePointEvent>
#include <QtMath>
#include "../include/EventManager.h"

EventManager::EventManager(ArrowSymbolWidget* arrow,
                           SpeedometerWidget* py_speed,
                           BatteryAndSpeedWidget* py_batspeed,
                           Blinkers* left_blinker,
                           Blinkers* right_blinker,
                           QStackedWidget* stackedWidget)
    : arrows(arrow), py_speed(py_speed), py_batspeed(py_batspeed), left_blinker(left_blinker), right_blinker(right_blinker), stackedWidget(stackedWidget)
{
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &EventManager::processKeyStates);
    updateTimer->start(80); // Every 80ms
}

bool EventManager::eventFilter(QObject* obj, QEvent* event)
{
    // Handle Mouse Press Button
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mousePos = static_cast<QMouseEvent*>(event);
        QPointF pressPosition = mousePos->position();

        mousePosition = pressPosition;
        return true;
    }
    // Handle Mouse Button Release
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mousePos = static_cast<QMouseEvent*>(event);
        QPointF releasePosition = mousePos->position();
        // Check for swipe params. If enough conditions, returns true for swipe.
        if (swipe(releasePosition))
            return true;
    }
    // Handle key events
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        pressedKeys.insert(keyEvent->key());
        return true;
    }
    else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        pressedKeys.remove(keyEvent->key());
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void EventManager::processKeyStates()
{
    for (int key : pressedKeys) {
        switch (key) {
            case Qt::Key_Left:
            case Qt::Key_Right:
            case Qt::Key_Up:
                if (arrows)
                    arrows->changeDirection(key);
                break;
            case Qt::Key_Space:
            case Qt::Key_Down:
                if (py_speed)
                    py_speed->accelerate(key);
                if (py_batspeed)
                    py_batspeed->accelerate(key);
                break;
            case Qt::Key_L:
                if (left_blinker && right_blinker)
                {
                    if (!left_blinker->get_blinking())
                        left_blinker->turnOnBlinkers(true);
                    else
                        left_blinker->turnOnBlinkers(false);
                    right_blinker->turnOnBlinkers(false);
                }
                break;
            case Qt::Key_R:
                if (left_blinker && right_blinker)
                {
                    if (!right_blinker->get_blinking())
                        right_blinker->turnOnBlinkers(true);
                    else
                        right_blinker->turnOnBlinkers(false);
                    left_blinker->turnOnBlinkers(false);
                }
                break;
            case Qt::Key_B:
                if (right_blinker && left_blinker)
                {
                    if (right_blinker->get_blinking() && left_blinker->get_blinking())
                    {
                        left_blinker->turnOnBlinkers(false);
                        right_blinker->turnOnBlinkers(false);
                        break;
                    }
                        left_blinker->turnOnBlinkers(false);
                        right_blinker->turnOnBlinkers(false);
                        left_blinker->turnOnBlinkers(true);
                        right_blinker->turnOnBlinkers(true);
                }
                break;
            default:
                break;
        }
    }
}

bool    EventManager::swipe(QPointF releasePosition)
{
    bool swipeRange = qFabs(mousePosition.rx() - releasePosition.rx()) > 100;
    bool verticalRange = qFabs(mousePosition.ry() - releasePosition.ry()) < 200;

    if (mousePosition.rx() < releasePosition.rx() && swipeRange && verticalRange ) {
        int prevIndex = std::max(0, stackedWidget->currentIndex() - 1);
        stackedWidget->setCurrentIndex(prevIndex);

        return true;
    }
    else if (mousePosition.rx() > releasePosition.rx() && swipeRange && verticalRange ) {
        int nextIndex = std::min(stackedWidget->count() - 1, stackedWidget->currentIndex() + 1);
        stackedWidget->setCurrentIndex(nextIndex);

        return true;
    }
    return false;
}

QStackedWidget*  EventManager::getStackedWidget(void) const
{
    return stackedWidget;
}

EventManager::~EventManager() {}
