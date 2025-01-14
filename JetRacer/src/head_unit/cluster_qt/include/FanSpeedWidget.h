#ifndef FANSPEEDWIDGET_H
#define FANSPEEDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>  // Include for key events

class FanSpeedWidget : public QWidget {

    public:
        FanSpeedWidget(QWidget* parent = nullptr);
        ~FanSpeedWidget();
    protected:
        void paintEvent(QPaintEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;  // Added keyPressEvent

    private:
        int currentSpeed;
        void drawScale(QPainter& painter, int centerX, int centerY, int radius);
        void drawNeedle(QPainter& painter, int centerX, int centerY, int radius);
        void drawCentralNumber(QPainter& painter, int centerX, int centerY);

    private slots:
        void updateSpeed();
};

#endif // SPEEDOMETERWIDGET_H
