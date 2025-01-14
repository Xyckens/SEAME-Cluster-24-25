#include "../include/BatteryAndSpeedWidget.h"
#include <iostream>


BatteryAndSpeedWidget::BatteryAndSpeedWidget(QWidget* parent)
    : QWidget(parent), currentSpeed(0)
{
    updateLevel();
    setFocusPolicy(Qt::StrongFocus); 
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BatteryAndSpeedWidget::updateLevel);
    timer->start(800);
}

void BatteryAndSpeedWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap image("assets/icons/battery.png");
    painter.setRenderHint(QPainter::Antialiasing);
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = std::min(width(), height()) / 2 - 20;

    int imageWidth = 25;
    int imageHeight = 25;
    int imageX = centerX - 30 - imageWidth / 2;
    int imageY = centerY + imageWidth / 2;

    painter.drawPixmap(imageX, imageY, imageWidth, imageHeight, image);

    drawCentralNumber(painter, centerX + 100, centerY);
    drawBatteryNumber(painter, imageX + 15, imageY + 55);
    drawScale(painter, centerX, centerY, radius);
}

void BatteryAndSpeedWidget::drawScale(QPainter& painter, int centerX, int centerY, int radius)
{
    double startAngle = -40;
    double endAngle = 40;
    QFont font("Arial", 20, QFont::Bold); 
    painter.setPen(QPen(Qt::white, 6));
    painter.setFont(font);
    drawBars(painter, centerX + 150, centerY + 30, radius * 1.2, startAngle, endAngle, 100);
    painter.setPen(QPen(Qt::black, 15));

    QRect rect(centerX, centerY - 80, radius, radius * 1.3);

    int startAngle1 = 135 * 16;
    int spanAngle1 = 90 * 16;

}

void BatteryAndSpeedWidget::drawBars(QPainter& painter, int centerX, int centerY, int radius, double startAngle, double endAngle, int Level)
{
    int numBars = Level / 2;
    int barWidth = 6; 
    int innerRadius = radius - 80;
    int outerRadius = radius - 70;
    int activeBars = static_cast<int>(currentLevel);

    for (int i = 0; i <= numBars; ++i)
    {
        double angle = startAngle + (endAngle - startAngle) * (double(i) / numBars);
        double rad = qDegreesToRadians(angle);

        int xOuter = centerX - std::cos(rad) * outerRadius;
        int yOuter = centerY - std::sin(rad) * outerRadius;
        int xInner = centerX - std::cos(rad) * innerRadius;
        int yInner = centerY - std::sin(rad) * innerRadius;
    
        QColor barColor = (i * 2 <= activeBars) ? calculateBarColor(i * 2) : QColor(50, 50, 50);
    
        painter.setPen(QPen(barColor, barWidth, Qt::SolidLine, Qt::SquareCap));
        painter.drawLine(xInner, yInner, xOuter, yOuter);
    }
}

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

void BatteryAndSpeedWidget::updateLevel()
{
    const std::string command = "upower -i $(upower -e | grep BAT) | grep -E \"percentage\" | awk '{print $2}'";
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe)
        throw std::runtime_error("Failed to run command");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        result += buffer.data();
    result.erase(result.find_last_not_of(" \t\n\r") + 1);
    try
    {
        currentLevel = std::stof(result);
    }
    catch(const std::exception& e)
    {
        std::cerr << "battery level not valid\n";
        currentLevel = 56;
    }
    update();
}

BatteryAndSpeedWidget::~BatteryAndSpeedWidget() {}

void BatteryAndSpeedWidget::drawCentralNumber(QPainter& painter, int centerX, int centerY)
{
    QFont font("Arial", 80, QFont::Bold); 
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
    QString speedText = QString::number(currentSpeed);
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(speedText);

    int x = centerX - textRect.width() / 2;
    int y = centerY + textRect.height() / 2 - 20;
    painter.drawText(x, y, speedText);

    QFont smallFont("Arial", 28, QFont::Bold);
    painter.setFont(smallFont);

    QFontMetrics smallMetrics(smallFont);
    int kphWidth = smallMetrics.horizontalAdvance("KPH");
    int kphX = centerX - kphWidth / 2;
    int kphY = y + textRect.height() - 60; 

    painter.drawText(kphX, kphY, "KPH");
}

void BatteryAndSpeedWidget::drawBatteryNumber(QPainter& painter, int centerX, int centerY)
{
    QFont font("Arial", 10, QFont::Bold); 
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
    QString speedText = QString::number(currentLevel) + " %";

    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(speedText);

    int x = centerX - textRect.width() / 2;
    int y = centerY + textRect.height() / 2 - 20;
    painter.drawText(x, y, speedText);
}

void BatteryAndSpeedWidget::accelerate(int forward_key)
{
    if (forward_key == Qt::Key_Space)
    {
        currentSpeed += 2; 
        if (currentSpeed > 160)
            currentSpeed = 160; 
        update(); 
    }


    if (forward_key == Qt::Key_Down)
    {
        currentSpeed -= 2; 
        if (currentSpeed <= 0)
            currentSpeed = 0; 
        update(); 
    }
}

void BatteryAndSpeedWidget::updateSpeed()
{
    update();
}
