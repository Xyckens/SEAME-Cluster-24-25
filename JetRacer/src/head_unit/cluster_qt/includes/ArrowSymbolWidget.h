#ifndef ARROWSYMBOLWIDGET_H
#define ARROWSYMBOLWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>
#include "Colors.h"
#include <QTimer>

class ArrowSymbolWidget : public QWidget
{
    public:
        ArrowSymbolWidget(QWidget* parent = nullptr, int x = 0, int y = 0, int width = 0, int height = 0);
        ~ArrowSymbolWidget() override;

        void    changeColor(int  array_index);
        void    turnOnLanes(bool left, bool right);
        void    changeDirection(double ang, QString dir);
        //void    proximityAlert(bool prox);
        void    proximityAlert(void);


    protected:
        void    paintEvent(QPaintEvent* event) override;

    private:
        void    variangle(void);
        void    forwardArrows(QPainter& painter);
        void    backwardsArrows(QPainter& painter);

        QColor  main_color;
        QColor  accent_color;
        QColor  alphabet_color;
        QColor  left_color;
        QColor  right_color;
        QString direction;

        QPixmap car;

        QPixmap lane_alert;
        QPixmap obj_prox_alert;
        QPixmap lane_img[4];
        QPixmap obj_prox_img[4];
        
        double  angle;
        double  step;

        Color   color1;

        bool    proximity;
        bool    lane_off;
};

#endif