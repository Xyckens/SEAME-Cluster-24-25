#include "../include/ArrowSymbolWidget.h"

ArrowSymbolWidget::ArrowSymbolWidget(QWidget* parent, std::string input)
    : QWidget(parent)
{
    m_greenL = false;
    m_greenR = false;
    m_greenV = false;
    m_drawVerticalArrows = true;
    m_drawLeftCurve = true;
    m_drawRightCurve = true;
    if (input == "front")
        m_greenV = true;
    else if (input == "left")
        m_greenL = true;
    else if (input == "right")
        m_greenR = true;
    setFocusPolicy(Qt::StrongFocus);
}

ArrowSymbolWidget::~ArrowSymbolWidget() {}

void ArrowSymbolWidget::setDrawVerticalArrows(bool enabled)
{
    m_drawVerticalArrows = enabled;
    update(); // Trigger a repaint
}

void ArrowSymbolWidget::setDrawLeftCurve(bool enabled)
{
    m_drawLeftCurve = enabled;
    update(); // Trigger a repaint
}

void ArrowSymbolWidget::setDrawRightCurve(bool enabled)
{
    m_drawRightCurve = enabled;
    update(); // Trigger a repaint
}

void ArrowSymbolWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    if (m_drawLeftCurve) {
        if (m_greenL == false)
            drawLeftCurve(painter, "black");
        else
            drawLeftCurve(painter, "green");
    }

    if (m_drawRightCurve) {
        if (m_greenR == false) 
            drawRightCurve(painter, "black");
        else
            drawRightCurve(painter, "green");

    }
    if (m_drawVerticalArrows) {
        if (m_greenV == false)
            drawVerticalArrows(painter, "black");
        else
            drawVerticalArrows(painter, "green");

    }
}

void ArrowSymbolWidget::drawVerticalArrows(QPainter& painter, std::string color)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int stemLength = 100;
    int arrowSize = 10;

    // Draw the central vertical arrow
    if (color == "black")
        painter.setPen(QPen(Qt::black,10));
    else
        painter.setPen(QPen(Qt::green,10));

    painter.drawLine(centerX, centerY, centerX, centerY - stemLength);
    drawArrowhead(painter, centerX, centerY - stemLength, 90, arrowSize, color);

    painter.drawLine(centerX, centerY, centerX, centerY + stemLength);
    drawArrowhead(painter, centerX, centerY + stemLength, 270, arrowSize, color);
}

void ArrowSymbolWidget::drawLeftCurve(QPainter& painter, std::string color)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int arrowSize = 10;

    if (color == "black")
        painter.setPen(QPen(Qt::black,10));
    else
        painter.setPen(QPen(Qt::green,10));

    QPainterPath leftCurve;
    leftCurve.moveTo(centerX, centerY + 40);
    leftCurve.cubicTo(centerX - 50, centerY - 40, centerX - 70, centerY - 50, centerX - 120, centerY - 60);
    painter.drawPath(leftCurve);
    drawArrowhead(painter, centerX - 120, centerY - 55, 135, arrowSize, color);
}

void ArrowSymbolWidget::drawRightCurve(QPainter& painter, std::string color)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int arrowSize = 10;

    if (color == "black")
        painter.setPen(QPen(Qt::black,10));
    else
        painter.setPen(QPen(Qt::green,10));

    QPainterPath rightCurve;
    rightCurve.moveTo(centerX, centerY + 40);
    rightCurve.cubicTo(centerX + 50, centerY - 40, centerX + 70, centerY - 50, centerX + 120, centerY - 60);
    painter.drawPath(rightCurve);
    drawArrowhead(painter, centerX + 120, centerY - 55, 45, arrowSize, color);
}

void ArrowSymbolWidget::drawArrowhead(QPainter& painter, int x, int y, double angle, int size, std::string color)
{
    QPolygon arrowHead;
    double radAngle = qDegreesToRadians(angle);
    double radLeft = qDegreesToRadians(angle + 150);
    double radRight = qDegreesToRadians(angle - 150);

    if (color == "black")
        painter.setPen(QPen(Qt::black,10));
    else
        painter.setPen(QPen(Qt::green,10));

    size += 20;
    arrowHead << QPoint(x + std::cos(radRight) * size, y + std::sin(radRight) * size);
    arrowHead << QPoint(x + std::cos(radLeft) * size, y + std::sin(radLeft) * size);
    arrowHead << QPoint(x, y);

    painter.drawPolygon(arrowHead);
}

void ArrowSymbolWidget::changeDirection(int movement_key)
{
   // FUTURE ROS2 CODE
}

bool    ArrowSymbolWidget::getM_greenL(void)
{
    return m_greenL;
}

bool    ArrowSymbolWidget::getM_greenR(void)
{
    return m_greenR;
}

bool    ArrowSymbolWidget::getM_greenV(void)
{
    return m_greenV;
}
