#include "../includes/BatteryWidget.h"


/**
 * @brief Constructs a BatteryWidget with optional position and size parameters.
 *
 * @param parent The parent widget.
 * @param x The x-coordinate of the widget's position (default is 0).
 * @param y The y-coordinate of the widget's position (default is 0).
 * @param width The width of the widget (default is 0).
 * @param height The height of the widget (default is 0).
 */
BatteryWidget::BatteryWidget(QWidget* parent, int x, int y, int width, int height)
    : QWidget(parent), current_level(0)
{
    color1 = Color();
    main_color = color1.main_color;
    accent_color = color1.accent_color;
    alphabet_color = color1.alphabet_color;
    setGeometry(x, y, width, height);

}

/**
 * @brief Handles the painting of the widget.
 *
 * This method is overridden from QWidget and is called whenever the widget needs to be redrawn.
 *
 * @param event The paint event.
 */
void BatteryWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int center_x = width() / 2;
    int center_y = height() / 2;
    int radius = std::min(width(), height()) / 2 - 20;
    drawScale(painter, center_x, center_y, radius);
    drawCentralNumber(painter, center_x, center_y);
}

/**
 * @brief Draws the scale on the widget.
 *
 * @param painter The QPainter object used for drawing.
 * @param center_x The x-coordinate of the center of the scale.
 * @param center_y The y-coordinate of the center of the scale.
 * @param radius The radius of the scale.
 */
void BatteryWidget::drawScale(QPainter& painter, int center_x, int center_y, int radius)
{
    int minLevel = 0, maxLevel = 100, step = 10;
    double startAngle = -45; // Start angle for 0 Level (bottom left)
    double endAngle = 225;     // End angle for max Level (bottom right)
    QFont font("Arial", 20, QFont::Bold);
    painter.setFont(font);
    painter.setPen(QPen(alphabet_color, 6));
    for (int Level = minLevel; Level <= maxLevel; Level += step / 2)
    {
        double angle = startAngle + (endAngle - startAngle) * (double(Level) / maxLevel);
        double rad = qDegreesToRadians(angle);
        int xOuter = center_x - std::cos(rad) * radius;
        int yOuter = center_y - std::sin(rad) * radius;
        int xInner = center_x - std::cos(rad) * (radius - 6);
        int yInner = center_y - std::sin(rad) * (radius - 6);
        int xLabel = center_x - std::cos(rad) * (radius - 55) - 8;
        int yLabel = center_y - std::sin(rad) * (radius - 55);
    }
    drawBars(painter, center_x, center_y, radius, startAngle, endAngle, 100);
    painter.setPen(QPen(main_color, 15));
    painter.drawEllipse(center_x - radius, center_y - radius, 2 * radius, 2 * radius);
    int smaller_r = radius - 65;
    painter.drawEllipse(center_x - smaller_r, center_y - smaller_r, 2 * smaller_r, 2 * smaller_r);
}

/**
 * @brief Draws the battery bars on the widget.
 *
 * @param painter The QPainter object used for drawing.
 * @param center_x The x-coordinate of the center of the bars.
 * @param center_y The y-coordinate of the center of the bars.
 * @param radius The radius of the bars.
 * @param startAngle The starting angle of the bars.
 * @param endAngle The ending angle of the bars.
 * @param Level The current battery level.
 */
void BatteryWidget::drawBars(QPainter& painter, int center_x, int center_y, int radius, double startAngle, double endAngle, int Level)
{
    int numBars = Level / 2;
    int barWidth = 6; 
    int innerRadius = radius - 60;
    int outerRadius = radius - 5;

    int activeBars = static_cast<int>(current_level);

    for (int i = 0; i <= numBars; ++i) {
        double angle = startAngle + (endAngle - startAngle) * (double(i) / numBars);
        double rad = qDegreesToRadians(angle);

        int xOuter = center_x - std::cos(rad) * outerRadius;
        int yOuter = center_y - std::sin(rad) * outerRadius;
        int xInner = center_x - std::cos(rad) * innerRadius;
        int yInner = center_y - std::sin(rad) * innerRadius;

    
        QColor barColor = (i * 2 <= activeBars) ? calculateBarColor(i * 2) : QColor(50, 50, 50);

    
        painter.setPen(QPen(barColor, barWidth, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(xInner, yInner, xOuter, yOuter);
    }
}

/**
 * @brief Calculates the color of the battery bars based on the current value.
 *
 * @param value The current battery level.
 * @return The QColor representing the bar color.
 */
QColor BatteryWidget::calculateBarColor(int value)
{
    if (value < 5) {
        return QColor(139, 0, 0); // Dark red
    } else if (value < 50) {
        // Gradient from dark red to yellow
        int red = 139 + (255 - 139) * (value - 5) / 45;
        int green = (255 - 139) * (value - 5) / 45;
        return QColor(red, green, 0);
    } else if (value < 80) {
        // Gradient from yellow to green
        int red = 255 - (255 * (value - 50) / 30);
        int green = 255;
        return QColor(red, green, 0);
    } else {
        // Green
        return QColor(0, 255, 0);
    }
}

/**
 * @brief Draws the central number on the widget.
 *
 * @param painter The QPainter object used for drawing.
 * @param center_x The x-coordinate of the center of the number.
 * @param center_y The y-coordinate of the center of the number.
 */
void BatteryWidget::drawCentralNumber(QPainter& painter, int center_x, int center_y)
{
    QFont font("Arial", 30, QFont::Bold);
    painter.setFont(font);
    painter.setPen(QPen(alphabet_color));
    QString LevelText = QString::number(current_level);
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(LevelText);
    int x = center_x - textRect.width() / 2;
    int y = center_y + textRect.height() / 2 - 20;
    painter.drawText(x, y, LevelText);
    QFont smallFont("Arial", 14, QFont::Bold); 
    painter.setFont(smallFont);
    QFontMetrics smallMetrics(smallFont);
    int kphWidth = smallMetrics.horizontalAdvance("%");
    int kphX = center_x - kphWidth / 2;
    int kphY = y + textRect.height() - 20; 
    painter.drawText(kphX, kphY, "%");
}

/**
 * @brief Updates the battery level displayed on the widget.
 *
 * This slot is triggered to refresh the battery level display. It can be connected to a signal
 * that provides real-time battery level updates.
 */
void BatteryWidget::updateLevel()
{
    update();
}

/**
 * @brief Sets the current battery level to be displayed.
 *
 * @param battery The battery level value to display (typically 0-100).
 */
void BatteryWidget::setCurrentLevel(int battery)
{
    if (battery == current_level)
        return ;
    current_level = battery;
    update();
}

/**
 * @brief Changes the color scheme of the widget.
 *
 * @param array_index The index of the color scheme in the `image_array`.
 */
void    BatteryWidget::changeColor(int  array_index)
{
    main_color = color1.main_color_array[array_index];
    accent_color = color1.accent_color_array[array_index];
    alphabet_color = color1.alphabet_color_array[array_index];   
    update();
}

/**
 * @brief Destroys the BatteryWidget.
 */
BatteryWidget::~BatteryWidget() {}
