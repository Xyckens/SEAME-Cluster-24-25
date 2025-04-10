#include "SpeedometerWidget.h"

/**
 * @brief Constructs a SpeedometerWidget with the specified parameters.
 *
 * This constructor initializes the speedometer widget, sets its geometry, and configures
 * its colors and focus policy.
 *
 * @param parent The parent widget.
 * @param x The x-coordinate of the widget's position (default is 0).
 * @param y The y-coordinate of the widget's position (default is 0).
 * @param width The width of the widget (default is 0).
 * @param height The height of the widget (default is 0).
 */
SpeedometerWidget::SpeedometerWidget(QWidget* parent, int x, int y, int width, int height)
    : QWidget(parent), current_speed(0)
{
    color1 = Color();
    unit = "DPS";
    maxSpeed = 220;
    main_color = color1.main_color;
    accent_color = color1.accent_color;
    alphabet_color = color1.alphabet_color;
    setGeometry(x, y, width, height);
    setFocusPolicy(Qt::StrongFocus);
}

/**
 * @brief Handles the painting of the speedometer.
 *
 * This method is overridden from QWidget and is called whenever the widget needs to be redrawn.
 * It draws the circular scale, needle, and central speed number.
 *
 * @param event The paint event.
 */
void SpeedometerWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = std::min(width(), height()) / 2 - 20;
    drawScale(painter, centerX, centerY, radius);
    drawNeedle(painter, centerX, centerY, radius);
    drawCentralNumber(painter, centerX, centerY);
}

/**
 * @brief Draws the circular scale of the speedometer.
 *
 * This method draws the outer circle, tick marks, and labels for the speedometer scale.
 *
 * @param painter The QPainter object used for drawing.
 * @param centerX The x-coordinate of the center of the scale.
 * @param centerY The y-coordinate of the center of the scale.
 * @param radius The radius of the scale.
 */
void SpeedometerWidget::drawScale(QPainter& painter, int centerX, int centerY,
                                  int radius)
{
    painter.setPen(QPen(main_color, 15));
    painter.drawEllipse(centerX - radius, centerY - radius, 2 * radius,
                        2 * radius);
    painter.setPen(QPen(accent_color, 15));
    int smaller_r = radius - 10;
    painter.drawEllipse(centerX - smaller_r, centerY - smaller_r, 2 * smaller_r,
                        2 * smaller_r);
    painter.setPen(QPen(main_color, 15));
    smaller_r -= 80;
    painter.drawEllipse(centerX - smaller_r, centerY - smaller_r, 2 * smaller_r,
                        2 * smaller_r);
    int minSpeed = 0, step = 20;
    double startAngle = -45; // Start angle for 0 speed (bottom left)
    double endAngle = 225;   // End angle for max speed (bottom right)
    QFont font("Arial", 20);
    painter.setPen(QPen(alphabet_color, 6));
    painter.setFont(font);
    for (int speed = minSpeed; speed <= maxSpeed; speed += step / 2)
    {
        double angle =
            startAngle + (endAngle - startAngle) * (double(speed) / maxSpeed);
        double rad = qDegreesToRadians(angle);
        int xOuter = centerX - std::cos(rad) * radius;
        int yOuter = centerY - std::sin(rad) * radius;
        int xInner = centerX - std::cos(rad) * (radius - 6);
        int yInner = centerY - std::sin(rad) * (radius - 6);
        if (speed == 50 || speed == 70 || speed == 120)
            painter.setPen(QPen(Qt::red, 6));
        else if (speed / 10 % 2 != 0)
            painter.setPen(QPen(Qt::gray, 6));
        painter.drawLine(xOuter, yOuter, xInner, yInner);
        painter.setPen(QPen(alphabet_color, 2));
        int xLabel = centerX - std::cos(rad) * (radius - 55) - 8;
        int yLabel = centerY - std::sin(rad) * (radius - 55);
        if (speed / 10 % 2 == 0)
            painter.drawText(xLabel - 10, yLabel + 5, QString::number(speed));
    }
}

/**
 * @brief Draws the needle of the speedometer.
 *
 * This method draws the needle that indicates the current speed on the speedometer.
 *
 * @param painter The QPainter object used for drawing.
 * @param centerX The x-coordinate of the center of the needle.
 * @param centerY The y-coordinate of the center of the needle.
 * @param radius The radius of the needle's arc.
 */
void SpeedometerWidget::drawNeedle(QPainter& painter, int centerX, int centerY,
                                   int radius)
{
    double startAngle = -45; // Start angle for 0 speed (bottom left)
    double endAngle = 225;   // End angle for max speed (bottom right)
    double angle =
        startAngle + (endAngle - startAngle) * (double(current_speed) / maxSpeed);
    double rad = qDegreesToRadians(angle);
    int xStart = centerX - std::cos(rad) * (radius - 80);
    int yStart = centerY - std::sin(rad) * (radius - 80);
    int xEnd = centerX - std::cos(rad) * (radius - 5);
    int yEnd = centerY - std::sin(rad) * (radius - 5);
    painter.setPen(QPen(Qt::red, 4));
    painter.drawLine(xStart, yStart, xEnd, yEnd);
}

/**
 * @brief Draws the central speed number and unit.
 *
 * This method draws the current speed value and unit in the center
 * of the speedometer.
 *
 * @param painter The QPainter object used for drawing.
 * @param centerX The x-coordinate of the center of the number.
 * @param centerY The y-coordinate of the center of the number.
 */
void SpeedometerWidget::drawCentralNumber(QPainter& painter, int centerX,
                                          int centerY)
{

    QFont font("Arial", 30, QFont::Bold);
    painter.setFont(font);
    painter.setPen(QPen(alphabet_color));
    QString speedText = QString::number(current_speed);

    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(speedText);

    int x = centerX - textRect.width() / 2;
    int y = centerY + textRect.height() / 2 - 20;
    painter.drawText(x, y, speedText);

    QFont smallFont("Arial", 14, QFont::Bold);
    painter.setFont(smallFont);

    QFontMetrics smallMetrics(smallFont);
    int kphWidth = smallMetrics.horizontalAdvance(unit);
    int kphX = centerX - kphWidth / 2;
    int kphY =
        y + textRect.height() - 30;

    painter.drawText(kphX, kphY, unit);
}

/**
 * @brief Changes the color scheme of the speedometer.
 *
 * This method updates the colors of the speedometer based on the
 * provided array index.
 *
 * @param array_index The index of the color scheme in the `color1` arrays.
 */
void    SpeedometerWidget::changeColor(int  array_index)
{
    main_color = color1.main_color_array[array_index];
    accent_color = color1.accent_color_array[array_index];
    alphabet_color = color1.alphabet_color_array[array_index];   
    update();
}

/**
 * @brief Toggles between speed units.
 *
 * This method switches the speed unit and adjusts the maximum speed and current speed
 * accordingly.
 */
void    SpeedometerWidget::changeUnits(void)
{
    if (unit == "DPS"){
        unit = "FPS";
        maxSpeed = 160;
        current_speed *= 0.6214;
    } else {
        unit = "DPS";
        maxSpeed = 220;
        current_speed *= 1.609;
    }
    update();
}

/**
 * @brief A Getter that returns the current speed unit.
 *
 * @return The current speed unit as a QString (e.g., "DPS" or "FPS").
 */
QString SpeedometerWidget::getUnit()
{
    return unit;
}

/**
 * @brief A Setter that updates the current speed of the speedometer.
 *
 * This method updates the current speed value and triggers a repaint of the widget.
 *
 * @param speed The new speed value.
 */
void SpeedometerWidget::setCurrentSpeed(int speed)
{
    if (speed == current_speed || (unit == "MPH" && speed * 0.614 == current_speed))
        return ;
    if (unit == "MPH")
        current_speed = 0.6214 * speed;
    else
        current_speed = speed;
    update();
}

/**
 * @brief A Getter that returns the current speed of the speedometer.
 *
 * @return The current speed as an integer.
 */

/**
 * @brief Destroys the SpeedometerWidget.
 *
 * This destructor cleans up any resources used by the widget.
 */
SpeedometerWidget::~SpeedometerWidget() {}
