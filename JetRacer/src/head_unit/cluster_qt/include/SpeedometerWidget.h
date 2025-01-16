#ifndef SPEEDOMETERWIDGET_H
#define SPEEDOMETERWIDGET_H

#include "Colors.h"
#include <QKeyEvent> // Include for key events
#include <QPainter>
#include <QTimer>
#include <QWidget>

class SpeedometerWidget : public QWidget
{

    public:
        SpeedometerWidget(QWidget* parent = nullptr);
        ~SpeedometerWidget();

        void setCurrentSpeed(int speed);
        void accelerate(int forward_key);
        QColor main_color;
        QColor accent_color;
        QColor alphabet_color;

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        int currentSpeed;
        Color color1;
        void drawScale(QPainter& painter, int centerX, int centerY, int radius);
        void drawNeedle(QPainter& painter, int centerX, int centerY,
                        int radius);
        void drawCentralNumber(QPainter& painter, int centerX, int centerY);

    private slots:
        void updateSpeed();
};

#endif // SPEEDOMETERWIDGET_H
