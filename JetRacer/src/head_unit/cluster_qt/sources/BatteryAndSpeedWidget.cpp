#include "../includes/BatteryAndSpeedWidget.h"

/**
 * @brief Constructs a BatteryAndSpeedWidget with optional position and size parameters.
 *
 * @param parent The parent widget.
 * @param x The x-coordinate of the widget's position (default is 0).
 * @param y The y-coordinate of the widget's position (default is 0).
 * @param width The width of the widget (default is 0).
 * @param height The height of the widget (default is 0).
 */
BatteryAndSpeedWidget::BatteryAndSpeedWidget(QWidget* parent, int x, int y, int width, int height)
    : QWidget(parent), current_speed(0)
{
    color1 = Color();
    unit = "DPS";
    index = color1.counter;
    main_color = color1.main_color;
    accent_color = color1.accent_color;
    alphabet_color = color1.alphabet_color;
    setFocusPolicy(Qt::StrongFocus);
    setGeometry(x, y, width, height);
}

/**
 * @brief Handles the painting of the widget.
 *
 * This method is overridden from QWidget and is called whenever the widget needs to be redrawn.
 *
 * @param event The paint event.
 */
void BatteryAndSpeedWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    image_array[0] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/battery_p.png");
    image_array[1] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/battery_r.png");
    image_array[2] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/battery_i.png");
    image_array[3] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/battery_g.png");
    image = image_array[index];
    painter.setRenderHint(QPainter::Antialiasing);
    int centerX = 40;
    int centerY = height() / 4;
    int radius = std::max(width(), height()) / 2 - 20;

    int imageWidth = 25;
    int imageHeight = 25;
    int image_x = centerX - 25 - imageWidth / 2;
    int image_y = centerY + imageWidth / 2;

    painter.drawPixmap(image_x, image_y, imageWidth, imageHeight, image);

    drawCentralNumber(painter, centerX + 100, centerY);
    drawBatteryNumber(painter, image_x + 15, image_y + 55);
    drawScale(painter, centerX, centerY, radius);
}

/**
 * @brief Draws the scale on the widget.
 *
 * @param painter The QPainter object used for drawing.
 * @param centerX The x-coordinate of the center of the scale.
 * @param centerY The y-coordinate of the center of the scale.
 * @param radius The radius of the scale.
 */
void BatteryAndSpeedWidget::drawScale(QPainter& painter, int centerX,
                                      int centerY, int radius)
{
    double startAngle = -40;
    double endAngle = 40;
    QFont font("Arial", 20, QFont::Bold);
    painter.setPen(QPen(alphabet_color, 6));
    painter.setFont(font);
    drawBars(painter, centerX + 150, centerY + 30, radius * 1.2, startAngle,
             endAngle, 100);
}

/**
 * @brief Draws the battery or speed bars on the widget.
 *
 * @param painter The QPainter object used for drawing.
 * @param centerX The x-coordinate of the center of the bars.
 * @param centerY The y-coordinate of the center of the bars.
 * @param radius The radius of the bars.
 * @param startAngle The starting angle of the bars.
 * @param endAngle The ending angle of the bars.
 * @param Level The current battery level.
 */
void BatteryAndSpeedWidget::drawBars(QPainter& painter, int centerX,
                                     int centerY, int radius, double startAngle,
                                     double endAngle, int Level)
{
    int numBars = Level / 2;
    int barWidth = 6;
    int innerRadius = radius - 80;
    int outerRadius = radius - 70;
    int activeBars = static_cast<int>(current_level);

    for (int i = 0; i <= numBars; ++i)
    {
        double angle =
            startAngle + (endAngle - startAngle) * (double(i) / numBars);
        double rad = qDegreesToRadians(angle);

        int xOuter = centerX - std::cos(rad) * outerRadius;
        int yOuter = centerY - std::sin(rad) * outerRadius;
        int xInner = centerX - std::cos(rad) * innerRadius;
        int yInner = centerY - std::sin(rad) * innerRadius;

        QColor barColor = (i * 2 <= activeBars) ? calculateBarColor(i * 2)
                                                : QColor(50, 50, 50);

        painter.setPen(QPen(barColor, barWidth, Qt::SolidLine, Qt::SquareCap));
        painter.drawLine(xInner, yInner, xOuter, yOuter);
    }
}

/**
 * @brief Calculates the color of the battery or speed bars based on the current value.
 *
 * @param value The current value (e.g., speed or battery level).
 * @return The QColor representing the bar color.
 */
QColor BatteryAndSpeedWidget::calculateBarColor(int value)
{
    if (value < 5)
        return QColor(139, 0, 0);
    else if (value < 50)
    {
        int red = 139 + (255 - 139) * (value - 5) / 45;
        int green = (255 - 139) * (value - 5) / 45;
        return QColor(red, green, 0);
    }
    else if (value < 80)
    {
        int red = 255 - (255 * (value - 50) / 30);
        int green = 255;
        return QColor(red, green, 0);
    }
    else
        return QColor(0, 255, 0);
}

/**
 * @brief Updates the battery level displayed on the widget.
 */
void BatteryAndSpeedWidget::updateLevel()
{
    update();
}

/**
 * @brief Destroys the BatteryAndSpeedWidget.
 */
BatteryAndSpeedWidget::~BatteryAndSpeedWidget() {}

/**
 * @brief Draws the central number on the widget.
 *
 * @param painter The QPainter object used for drawing.
 * @param centerX The x-coordinate of the center of the number.
 * @param centerY The y-coordinate of the center of the number.
 */
void BatteryAndSpeedWidget::drawCentralNumber(QPainter& painter,
                                            int centerX, int centerY)
{
    QFont font("Arial", 80, QFont::Bold);
    painter.setFont(font);
    painter.setPen(QPen(alphabet_color));
    QString speedText = QString::number(current_speed);
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(speedText);

    int x = centerX - textRect.width() / 2;
    int y = centerY + textRect.height() / 2 - 20;
    painter.drawText(x, y, speedText);

    QFont smallFont("Arial", 28, QFont::Bold);
    painter.setFont(smallFont);

    QFontMetrics smallMetrics(smallFont);
    int kphWidth = smallMetrics.horizontalAdvance(unit);
    int kphX = centerX - kphWidth / 2;
    int kphY = y + textRect.height() - 60;

    painter.drawText(kphX, kphY, unit);
}

/**
 * @brief Draws the battery level number on the widget.
 *
 * @param painter The QPainter object used for drawing.
 * @param centerX The x-coordinate of the center of the number.
 * @param centerY The y-coordinate of the center of the number.
 */
void BatteryAndSpeedWidget::drawBatteryNumber(QPainter& painter,
                                            int centerX, int centerY)
{
    QFont font("Arial", 10, QFont::Bold);
    painter.setFont(font);
    painter.setPen(QPen(alphabet_color));
    QString speedText = QString::number(current_level) + " %";

    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(speedText);

    int x = centerX - textRect.width() / 2;
    int y = centerY + textRect.height() / 2 - 20;
    painter.drawText(x, y, speedText);
}

/**
 * @brief Simulates acceleration based on the forward key input.
 *
 * @param forward_key The key input representing acceleration.
 */
void BatteryAndSpeedWidget::accelerate(int forward_key)
{
    if (forward_key == Qt::Key_Space)
    {
        current_speed += 2;
        if (current_speed > 160)
            current_speed = 160;
        update();
    }

    if (forward_key == Qt::Key_Down)
    {
        current_speed -= 2;
        if (current_speed <= 0)
            current_speed = 0;
        update();
    }
}

/**
 * @brief Changes the color scheme of the widget.
 *
 * @param array_index The index of the color scheme in the `image_array`.
 */
void    BatteryAndSpeedWidget::changeColor(int  array_index)
{
    main_color = color1.main_color_array[array_index];
    accent_color = color1.accent_color_array[array_index];
    alphabet_color = color1.alphabet_color_array[array_index];
    index = array_index;
    update();
}

/**
 * @brief A setter for the current battery level to be displayed.
 *
 * @param battery The battery level value to display (0-100).
 */
void    BatteryAndSpeedWidget::setCurrentLevel(int battery)
{
    if (battery == current_level)
        return ;
    current_level = battery;
    update();
}

/**
 * @brief Toggles between different units.
 */
void    BatteryAndSpeedWidget::changeUnits(void)
{
    if (unit == "DPS"){
        unit = "FPS";
        current_speed *= 0.6214;
    } else {
        unit = "DPS";
        current_speed *= 1.609;
    }
    update();
}

/**
 * @brief A setter for the current speed to be displayed.
 *
 * @param speed The speed value to display.
 */
void BatteryAndSpeedWidget::setCurrentSpeed(int speed)
{
    if (speed == current_speed
    || (unit == "DPS" && speed * 0.6214 == current_speed))
        return ;
    if (unit == "FPS")
        current_speed = 0.6214 * speed;
    else
        current_speed = speed;
    update();
}
