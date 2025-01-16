#ifndef ARROWSYMBOLWIDGET_H
#define ARROWSYMBOLWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>  // Include for key events
#include "Colors.h"

class ArrowSymbolWidget : public QWidget
{
    public:
        ArrowSymbolWidget(QWidget* parent = nullptr, std::string input = "none");
        ~ArrowSymbolWidget();

        void setDrawVerticalArrows(bool enabled);
        void setDrawLeftCurve(bool enabled);
        void setDrawRightCurve(bool enabled);
        void changeDirection(int movement_key);
        QColor  main_color;
        QColor  accent_color;
        QColor  alphabet_color;

    protected:
        ArrowSymbolWidget();
        void paintEvent(QPaintEvent* event) override;

    private:
        void drawVerticalArrows(QPainter& painter, std::string color);
        void drawLeftCurve(QPainter& painter, std::string color);
        void drawRightCurve(QPainter& painter, std::string color);
        void drawArrowhead(QPainter& painter, int x, int y, double angle, int size, std::string color);

        bool m_drawVerticalArrows;
        bool m_drawLeftCurve;
        bool m_drawRightCurve;
        bool m_greenL;
        bool m_greenR;
        bool m_greenV;
        Color color1;
};

#endif // ARROWSYMBOLWIDGET_H
