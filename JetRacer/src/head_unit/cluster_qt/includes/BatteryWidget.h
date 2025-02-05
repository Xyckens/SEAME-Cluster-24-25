#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "Colors.h"
#include <QPainter>
#include <QTimer>
#include <QWidget>


/**
 * @brief This class is a custom QWidget that displays battery level information.
 * @details This widget provides a graphical representation of the current battery level.
 */
class BatteryWidget : public QWidget
{

    public:
        BatteryWidget(QWidget* parent = nullptr, int x = 0, int y = 0, int width = 0, int height = 0);
        ~BatteryWidget();

        void    changeColor(int  array_index);
        void    setCurrentLevel(int battery);
        
        QColor  main_color;
        QColor  accent_color;
        QColor  alphabet_color;

    protected:
        void    paintEvent(QPaintEvent* event) override;

    private:
        int     currentLevel;
        Color   color1;
        void    drawScale(QPainter& painter, int centerX, int centerY, int radius);
        void    drawCentralNumber(QPainter& painter, int centerX, int centerY);
        QColor  calculateBarColor(int value);
        void    drawBars(QPainter& painter, int centerX, int centerY, int radius,
                      double startAngle, double endAngle, int Level);

    private slots:
        void    updateLevel();
};

#endif // BATTERYWIDGET_H
