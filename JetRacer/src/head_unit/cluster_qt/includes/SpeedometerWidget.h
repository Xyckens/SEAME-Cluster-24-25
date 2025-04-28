#ifndef SPEEDOMETERWIDGET_H
#define SPEEDOMETERWIDGET_H

#include "Colors.h"
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>


/**
 * @brief The SpeedometerWidget class represents a custom widget for displaying speed.
 *
 * @details This widget provides a graphical representation of speed, including a circular scale,
 * a needle, and a central display for the current speed value. It supports customization
 * of colors, units, and maximum speed.
 */

class SpeedometerWidget : public QWidget
{

    public:
        SpeedometerWidget(QWidget* parent = nullptr, int x = 0, int y = 0,
                          int width = 0, int height = 0);
        ~SpeedometerWidget() override;

        void    setCurrentSpeed(int speed);
        int     getCurrentSpeed() const;

        void    accelerate(int forward_key);
        void    changeColor(int  array_index);
        void    changeUnits(void);
        QString getUnit();
        QColor  main_color;
        QColor  accent_color;
        QColor  alphabet_color;

    protected:
        void    paintEvent(QPaintEvent* event) override;

    private:
        int     current_speed;
        int     maxSpeed;

        QString unit;
        Color   color1;
        void    drawScale(QPainter& painter, int center_x, int center_y, int radius);
        void    drawNeedle(QPainter& painter, int center_x, int center_y,
                           int radius);
        void    drawCentralNumber(QPainter& painter, int center_x, int center_y);
};

#endif
