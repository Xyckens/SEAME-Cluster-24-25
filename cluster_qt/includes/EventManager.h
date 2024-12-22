#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QWidget>
#include "ArrowSymbolWidget.h"
#include "SpeedometerWidget.h"
#include "Blinkers.h"
#include <QSwipeGesture>
#include <QStackedWidget>
#include <QObject>
#include <QSet>
#include <QTimer>

class EventManager : public QWidget
{
    Q_OBJECT;

    private:
        ArrowSymbolWidget* arrows;
        SpeedometerWidget* py_speed;
        Blinkers* left_blinker;
        Blinkers* right_blinker;
        QSet<int> pressedKeys;
        QTimer* updateTimer;   // Used to check if a key is still being called
        QStackedWidget* stackedWidget;
        QPointF mousePosition;


    public:
        EventManager(ArrowSymbolWidget* arrow,
                     SpeedometerWidget* py_speed,
                     Blinkers* left_blinker,
                     Blinkers* right_blinker,
                     QStackedWidget* stackedWidget);

        QStackedWidget*  getStackedWidget(void) const;

    protected:
        bool    eventFilter(QObject* obj, QEvent* event) override;
        bool    swipe(QPointF releasePosition);

    private slots:
        void processKeyStates();
};

#endif // EVENTMANAGER_H
