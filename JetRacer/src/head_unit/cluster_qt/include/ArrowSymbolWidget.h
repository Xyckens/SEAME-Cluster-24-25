#ifndef ARROWSYMBOLWIDGET_H
#define ARROWSYMBOLWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QKeyEvent>  // Include for key events

class ArrowSymbolWidget : public QWidget
{
    public:
        ArrowSymbolWidget(QWidget* parent = nullptr, std::string input = "none");
        ~ArrowSymbolWidget();

        void setDrawVerticalArrows(bool enabled);
        void setDrawLeftCurve(bool enabled);
        void setDrawRightCurve(bool enabled);
        void changeDirection(int movement_key);

        bool    getM_greenL(void);
        bool    getM_greenR(void);
        bool    getM_greenV(void);

    protected:
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
};

#endif // ARROWSYMBOLWIDGET_H
