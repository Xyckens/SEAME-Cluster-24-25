#include "../include/BatteryWidget.h"

BatteryWidget::BatteryWidget(QWidget* parent)
    : QWidget(parent), currentLevel(100)
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BatteryWidget::updateLevel);
    timer->start(800); // Update Level every 100ms
}

void BatteryWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = std::min(width(), height()) / 2 - 20;
    // Draw circular scale
    drawScale(painter, centerX, centerY, radius);
    // Draw needle
    drawCentralNumber(painter, centerX, centerY);
}

void BatteryWidget::drawScale(QPainter& painter, int centerX, int centerY, int radius) {
    // Draw outer circle
    painter.setPen(QPen(Qt::black, 15));
    int smaller_r = radius - 10;
    painter.setPen(QPen(Qt::black, 15));
    
    // Draw tick marks and labels
    int minLevel = 0, maxLevel = 100, step = 10;
    double startAngle = -45; // Start angle for 0 Level (bottom left)
    double endAngle = 225;     // End angle for max Level (bottom right)
    QFont font("Arial", 20, QFont::Bold);  // Example: Arial, size 10, bold
    painter.setPen(QPen(Qt::white, 6));
    painter.setFont(font);
    for (int Level = minLevel; Level <= maxLevel; Level += step / 2) {
        double angle = startAngle + (endAngle - startAngle) * (double(Level) / maxLevel);
        double rad = qDegreesToRadians(angle);
        int xOuter = centerX - std::cos(rad) * radius;
        int yOuter = centerY - std::sin(rad) * radius;
        int xInner = centerX - std::cos(rad) * (radius - 6);
        int yInner = centerY - std::sin(rad) * (radius - 6);
        // Draw tick mark
        painter.setPen(QPen(Qt::white, 6));
        // Draw label
        int xLabel = centerX - std::cos(rad) * (radius - 55) - 8;
        int yLabel = centerY - std::sin(rad) * (radius - 55);
    }
        drawBars(painter, centerX, centerY, radius, startAngle, endAngle, 100);
    painter.setPen(QPen(Qt::black, 15));
    painter.drawEllipse(centerX - radius, centerY - radius, 2 * radius, 2 * radius);
    smaller_r -=  80;
    painter.drawEllipse(centerX - smaller_r, centerY - smaller_r, 2 * smaller_r, 2 * smaller_r);
}

void BatteryWidget::drawBars(QPainter& painter, int centerX, int centerY, int radius, double startAngle, double endAngle, int Level) {
    int numBars = Level / 2; // Total number of bars (from 0 to 100)
    int barWidth = 6;  // Width of each bar
    int innerRadius = radius - 80; // Inner radius for bars
    int outerRadius = radius - 5; // Outer radius for bars

    int activeBars = static_cast<int>(currentLevel); // Bars below the needle

    for (int i = 0; i <= numBars; ++i) {
        double angle = startAngle + (endAngle - startAngle) * (double(i) / numBars);
        double rad = qDegreesToRadians(angle);

        int xOuter = centerX - std::cos(rad) * outerRadius;
        int yOuter = centerY - std::sin(rad) * outerRadius;
        int xInner = centerX - std::cos(rad) * innerRadius;
        int yInner = centerY - std::sin(rad) * innerRadius;

        // Determine bar color based on the value
        QColor barColor = (i * 2 <= activeBars) ? calculateBarColor(i * 2) : QColor(50, 50, 50); // Gray for inactive bars

        // Draw the bar
        painter.setPen(QPen(barColor, barWidth, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(xInner, yInner, xOuter, yOuter);
    }
}

QColor BatteryWidget::calculateBarColor(int value) {
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

void BatteryWidget::drawCentralNumber(QPainter& painter, int centerX, int centerY) {
    // Set font and color for the Level number
    QFont font("Arial", 40, QFont::Bold);  // Large font for the Level
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
    QString LevelText = QString::number(currentLevel);
    // Calculate the bounding box for the Level text
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(LevelText);
    // Center the Level text
    int x = centerX - textRect.width() / 2;
    int y = centerY + textRect.height() / 2 - 20;
    painter.drawText(x, y, LevelText);
    // Set a smaller font and adjust position for "KPH"
    QFont smallFont("Arial", 14, QFont::Bold);  // Smaller font for the unit
    painter.setFont(smallFont);
    // Adjust the position to render "KPH" just below the Level number
    QFontMetrics smallMetrics(smallFont);
    int kphWidth = smallMetrics.horizontalAdvance("%");
    int kphX = centerX - kphWidth / 2; // Center-align "KPH"
    int kphY = y + textRect.height() - 30;  // Position "KPH" below the Level text
    // Draw "KPH"
    painter.drawText(kphX, kphY, "%");
}

void BatteryWidget::updateLevel() {
    // Command to get the battery level -- CURRENTLY WORKING ON ARCH, NEEDS TO BE ALTERED
    const std::string command = "upower -i $(upower -e | grep BAT) | grep -E \"percentage\" | awk '{print $2}'";

    // Create a pipe to read the command's output
    std::array<char, 128> buffer;
    std::string result;

    // Open the process using popen
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to run command");
    }

    // Read the output into the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // Remove any trailing whitespace or newline characters
    result.erase(result.find_last_not_of(" \t\n\r") + 1);

    // Convert the result to a float or integer if needed
    //currentLevel = std::stof(result); // or std::stoi(result) if it's an integer
    currentLevel = 35; // or std::stoi(result) if it's an integer

    update(); // Trigger a repaint
    // FUTURE ROS2 CODE
}

BatteryWidget::~BatteryWidget() {}
