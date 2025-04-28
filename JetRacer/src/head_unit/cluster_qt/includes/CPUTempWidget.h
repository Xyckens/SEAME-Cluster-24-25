#ifndef CPUTTEMPWIDGET_H
#define CPUTTEMPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include "Colors.h"

class CPUTempWidget : public QWidget {

    public:
        CPUTempWidget(QWidget* parent = nullptr, int x = 0, int y = 0,
                      int width = 0, int height = 0);
        ~CPUTempWidget() override;
        void    changeColor(int array_index);
        void    changeUnits();
        QColor  main_color;
        QColor  accent_color;
        QColor  alphabet_color;

    protected:
        void    paintEvent(QPaintEvent* event) override;

    private:
        int     current_speed;
        int     index;

        Color   color1;
        QString unit;
        QPixmap image;
        QPixmap image_array[4];

        void    drawScale(QPainter& painter, int center_x, int center_y, int radius);
        void    drawNeedle(QPainter& painter, int center_x, int center_y, int radius);
        void    drawCentralNumber(QPainter& painter, int center_x, int center_y);

    private slots:
        void    updateSpeed();
};

#endif
