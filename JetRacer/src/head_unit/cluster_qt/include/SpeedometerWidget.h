#ifndef SPEEDOMETERWIDGET_H
#define SPEEDOMETERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>  // Include for key events

class SpeedometerWidget : public QWidget {

    public:
        SpeedometerWidget(QWidget* parent = nullptr);
        ~SpeedometerWidget();

        void    accelerate(int forward_key);
        int     getCurrentSpeed(void);

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

#endif
