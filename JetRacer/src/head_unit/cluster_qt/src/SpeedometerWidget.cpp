#include "../include/SpeedometerWidget.h"

SpeedometerWidget::SpeedometerWidget(QWidget* parent)
    : QWidget(parent), currentSpeed(0)
{
    setFocusPolicy(Qt::StrongFocus);  // Ensure the widget can receive key events
}


void SpeedometerWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = std::min(width(), height()) / 2 - 20;
    // Draw circular scale
    drawScale(painter, centerX, centerY, radius);
    // Draw needle
    drawNeedle(painter, centerX, centerY, radius);
    drawCentralNumber(painter, centerX, centerY);
}

void SpeedometerWidget::drawScale(QPainter& painter, int centerX, int centerY, int radius) {
    // Draw outer circle
    painter.setPen(QPen(Qt::black, 15));
    painter.drawEllipse(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
    painter.setPen(QPen(QColor("#2A4C5C"), 15));
    int smaller_r = radius - 10;
    painter.drawEllipse(centerX - smaller_r, centerY - smaller_r, 2 * smaller_r, 2 * smaller_r);
    painter.setPen(QPen(Qt::black, 15));
    smaller_r -=  80;
    painter.drawEllipse(centerX - smaller_r, centerY - smaller_r, 2 * smaller_r, 2 * smaller_r);
    // Draw tick marks and labels
    int minSpeed = 0, maxSpeed = 160, step = 20;
    double startAngle = -45; // Start angle for 0 speed (bottom left)
    double endAngle = 225;     // End angle for max speed (bottom right)
    QFont font("Arial", 20, QFont::Bold);  // Example: Arial, size 10, bold
    painter.setPen(QPen(Qt::white, 6));
    painter.setFont(font);
    for (int speed = minSpeed; speed <= maxSpeed; speed += step / 2) {
        double angle = startAngle + (endAngle - startAngle) * (double(speed) / maxSpeed);
        double rad = qDegreesToRadians(angle);
        int xOuter = centerX - std::cos(rad) * radius;
        int yOuter = centerY - std::sin(rad) * radius;
        int xInner = centerX - std::cos(rad) * (radius - 6);
        int yInner = centerY - std::sin(rad) * (radius - 6);
        // Draw tick mark
        if (speed == 50 || speed == 70 || speed == 120)
            painter.setPen(QPen(Qt::red, 6));
        else if (speed / 10 % 2 != 0)
            painter.setPen(QPen(Qt::gray, 6));
        painter.drawLine(xOuter, yOuter, xInner, yInner);
        painter.setPen(QPen(Qt::white, 6));
        // Draw label
        int xLabel = centerX - std::cos(rad) * (radius - 55) - 8;
        int yLabel = centerY - std::sin(rad) * (radius - 55);
        if (speed / 10 % 2 == 0)
            painter.drawText(xLabel - 10, yLabel + 5, QString::number(speed));
    }
}

void SpeedometerWidget::drawNeedle(QPainter& painter, int centerX, int centerY, int radius) {
    double startAngle = -45; // Start angle for 0 speed (bottom left)
    double endAngle = 225;   // End angle for max speed (bottom right)
    // Calculate the angle based on current speed in clockwise direction
    double angle = startAngle + (endAngle - startAngle) * (double(currentSpeed) / 160);
    double rad = qDegreesToRadians(angle);
    int xStart = centerX - std::cos(rad) * (radius - 80);
    int yStart = centerY - std::sin(rad) * (radius - 80);
    int xEnd = centerX - std::cos(rad) * (radius - 5);
    int yEnd = centerY - std::sin(rad) * (radius - 5);
    QPen pen(Qt::red, 4);
    painter.setPen(pen);
    // Draw the needle
    painter.drawLine(xStart, yStart, xEnd, yEnd);
}

void SpeedometerWidget::drawCentralNumber(QPainter& painter, int centerX, int centerY) {
    // Set font and color for the speed number
    QFont font("Arial", 40, QFont::Bold);  // Large font for the speed
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
    QString speedText = QString::number(currentSpeed);
    // Calculate the bounding box for the speed text
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(speedText);
    // Center the speed text
    int x = centerX - textRect.width() / 2;
    int y = centerY + textRect.height() / 2 - 20;
    painter.drawText(x, y, speedText);
    // Set a smaller font and adjust position for "KPH"
    QFont smallFont("Arial", 14, QFont::Bold);  // Smaller font for the unit
    painter.setFont(smallFont);
    // Adjust the position to render "KPH" just below the speed number
    QFontMetrics smallMetrics(smallFont);
    int kphWidth = smallMetrics.horizontalAdvance("KPH");
    int kphX = centerX - kphWidth / 2; // Center-align "KPH"
    int kphY = y + textRect.height() - 30;  // Position "KPH" below the speed text
    // Draw "KPH"
    painter.drawText(kphX, kphY, "KPH");
}

void SpeedometerWidget::accelerate(int forward_key)
{
    // FUTURE ROS2
}

void SpeedometerWidget::updateSpeed() {
    update(); // Trigger a repaint
}

SpeedometerWidget::~SpeedometerWidget() {}

int     SpeedometerWidget::getCurrentSpeed(void)
{
    return currentSpeed;
}
