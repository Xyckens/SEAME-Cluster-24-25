#ifndef BATTERANDSPEEDYWIDGET_H
#define BATTERANDSPEEDYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

class BatteryAndSpeedWidget : public QWidget {

    public:
        BatteryAndSpeedWidget(QWidget* parent = nullptr);
        ~BatteryAndSpeedWidget();
    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        int currentLevel;
        int currentSpeed;
        void drawScale(QPainter& painter, int centerX, int centerY, int radius);
        void drawCentralNumber(QPainter& painter, int centerX, int centerY);
        void drawBatteryNumber(QPainter& painter, int centerX, int centerY);
        QColor calculateBarColor(int value);
        void drawBars(QPainter& painter, int centerX, int centerY, int radius, double startAngle, double endAngle, int Level);
        void accelerate(int forward_key);

    private slots:
        void updateLevel();
        void updateSpeed();
};

#endif