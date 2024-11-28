#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

class BatteryWidget : public QWidget {

    public:
        BatteryWidget(QWidget* parent = nullptr);
        ~BatteryWidget();
    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        int currentSpeed;
        void drawScale(QPainter& painter, int centerX, int centerY, int radius);
        void drawNeedle(QPainter& painter, int centerX, int centerY, int radius);
        void drawCentralNumber(QPainter& painter, int centerX, int centerY);

    private slots:
        void updateSpeed();
};

#endif // BATTERYWIDGET_H
