#ifndef FANSPEEDWIDGET_H
#define FANSPEEDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include "Colors.h"

class FanSpeedWidget : public QWidget {

    public:
        FanSpeedWidget(QWidget* parent = nullptr, int x = 0, int y = 0,
                       int width = 0, int height = 0);
        ~FanSpeedWidget() override;
        void    changeColor(int array_index);
        QColor  main_color;
        QColor  accent_color;
        QColor  alphabet_color;

    protected:
        void    paintEvent(QPaintEvent* event) override;

    private:
        int     current_speed;
        Color   color1;
        QPixmap image;
        QPixmap image_array[4]; 
        int     index;
        void    drawScale(QPainter& painter, int center_x, int center_y, int radius);
        void    drawNeedle(QPainter& painter, int center_x, int center_y, int radius);
        void    drawCentralNumber(QPainter& painter, int center_x, int center_y);

    private slots:
        void    updateSpeed();
};

#endif
