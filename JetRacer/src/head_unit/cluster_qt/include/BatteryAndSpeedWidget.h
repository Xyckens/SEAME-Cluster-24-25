#ifndef BATTERANDSPEEDYWIDGET_H
#define BATTERANDSPEEDYWIDGET_H

#include "Colors.h"
#include <QPainter>
#include <QTimer>
#include <QWidget>

class BatteryAndSpeedWidget : public QWidget
{

    public:
        BatteryAndSpeedWidget(QWidget* parent = nullptr);
        ~BatteryAndSpeedWidget();

        void setCurrentLevel(int level);

        void accelerate(int forward_key);
        QColor main_color;
        QColor accent_color;
        QColor alphabet_color;

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        int currentLevel;
        int currentSpeed;
        Color color1;
        void drawScale(QPainter& painter, int centerX, int centerY, int radius);
        void drawCentralNumber(QPainter& painter, int centerX, int centerY);
        void drawBatteryNumber(QPainter& painter, int centerX, int centerY);
        QColor calculateBarColor(int value);
        void drawBars(QPainter& painter, int centerX, int centerY, int radius,
                      double startAngle, double endAngle, int Level);

    private slots:
        void updateLevel();
        void updateSpeed();
};

#endif
