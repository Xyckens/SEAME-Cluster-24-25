#ifndef BATTERANDSPEEDYWIDGET_H
#define BATTERANDSPEEDYWIDGET_H

#include "Colors.h"
#include <QPainter>
#include <QTimer>
#include <QWidget>

/**
 * @brief This class is a custom QWidget that displays battery level and speed information.
 * @details The widget provides a graphical representation of the current battery level and speed. 
 * It supports customization of colors, units, and display styles. 
 */
class BatteryAndSpeedWidget : public QWidget
{

    public:
        BatteryAndSpeedWidget(QWidget* parent = nullptr, int x = 0, int y = 0,
                              int width = 0, int height = 0);
        ~BatteryAndSpeedWidget() override;

        void    setCurrentSpeed(int speed);
        void    setCurrentLevel(int battery);
        void    accelerate(int forward_key);
        void    changeColor(int  array_index);
        void    changeUnits(void);
        QColor  main_color;
        QColor  accent_color;
        QColor  alphabet_color;

    protected:
        void    paintEvent(QPaintEvent* event) override;

    private:
        int     current_level;
        int     current_speed;
        int     image_x;
        int     image_y;
        int     index = 0;

        Color   color1;
        QString unit;
        QPixmap image_array[4];
        QPixmap image;
        void    drawScale(QPainter& painter, int center_x, int center_y, int radius);
        void    drawCentralNumber(QPainter& painter, int center_x, int center_y);
        void    drawBatteryNumber(QPainter& painter, int center_x, int center_y);
        QColor  calculateBarColor(int value);
        void    drawBars(QPainter& painter, int center_x, int center_y, int radius,
                      double start_angle, double end_angle, int level);

    private slots:
        void    updateLevel();
};

#endif
