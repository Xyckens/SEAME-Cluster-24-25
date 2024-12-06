#include "includes/CPUTempWidget.h"

CPUTempWidget::CPUTempWidget(QWidget* parent)
    : QWidget(parent), currentLevel(0)
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CPUTempWidget::updateLevel);
    timer->start(800); // Update Level every 100ms
}


void CPUTempWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int centerX = (width() - 50) / 2;
    int centerY = (height() - 50) / 2;
    int radius = std::min((width() - 50), (height() - 50)) / 2 - 20;

    drawScale(painter, centerX, centerY, radius);

    drawNeedle(painter, centerX, centerY, radius);
    drawCentralNumber(painter, centerX, centerY);
}

void CPUTempWidget::drawScale(QPainter& painter, int centerX, int centerY, int radius) {

    painter.setPen(QPen(Qt::black, 8));
    painter.drawEllipse(centerX - radius, centerY - radius, 2 * radius, 2 * radius);

}

void CPUTempWidget::drawNeedle(QPainter& painter, int centerX, int centerY, int radius) {
    double startAngle = -45;
    double endAngle = 225;    

    double angle = startAngle + (endAngle - startAngle) * (double(currentLevel) / 160);
    double rad = qDegreesToRadians(angle);
    int xStart = centerX - std::cos(rad) * (radius - 3);
    int yStart = centerY - std::sin(rad) * (radius - 3);
    int xEnd = centerX - std::cos(rad) * (radius + 1);
    int yEnd = centerY - std::sin(rad) * (radius + 1);
    QPen pen(Qt::gray, 5);
    painter.setPen(pen);

    painter.drawLine(xStart, yStart, xEnd, yEnd);
}

void CPUTempWidget::drawCentralNumber(QPainter& painter, int centerX, int centerY) {

    QFont font("Arial", 20, QFont::Bold); 
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
    QString speedText = QString::number(currentLevel);

    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(speedText);

    int x = centerX - textRect.width() / 2;
    int y = centerY + textRect.height() / 2 - 10;
    painter.drawText(x, y, speedText);

    QFont smallFont("Arial", 10, QFont::Bold); 
    painter.setFont(smallFont);

    QFontMetrics smallMetrics(smallFont);
    int kphWidth = smallMetrics.horizontalAdvance("ºC");
    int kphX = centerX - kphWidth / 2;
    int kphY = y + textRect.height() - 20; 

    painter.drawText(kphX, kphY, "ºC");
    QPixmap image("assets/icons/cpu.png");
    if (!image.isNull()) {
        int imgWidth = 25; 
        int imgHeight = 25;
        int imgX = centerX - imgWidth / 2;
        int imgY = kphY + 20;
        painter.drawPixmap(imgX, imgY, imgWidth, imgHeight, image);
    }
}

void CPUTempWidget::updateLevel() {
    // Command to get the battery level -- command works on car
    const std::string command = "cat /sys/class/thermal/thermal_zone*/temp | sed -n '2p' | awk '{printf \"%.0f\\u00b0C\\n\", $1/1000}'";

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
    currentLevel = std::stof(result); // or std::stoi(result) if it's an integer

    update(); // Trigger a repaint
}

CPUTempWidget::~CPUTempWidget() {}
