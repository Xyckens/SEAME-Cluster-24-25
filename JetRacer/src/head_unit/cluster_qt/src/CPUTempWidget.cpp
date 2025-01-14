#include "../include/CPUTempWidget.h"

CPUTempWidget::CPUTempWidget(QWidget* parent)
    : QWidget(parent), currentSpeed(0)
{
    setFocusPolicy(Qt::StrongFocus); 
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

    double angle = startAngle + (endAngle - startAngle) * (double(currentSpeed) / 160);
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
    QString speedText = QString::number(currentSpeed);

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

void CPUTempWidget::keyPressEvent(QKeyEvent* event)
{

    if (event->key() == Qt::Key_Space) {
        currentSpeed += 2; 
        if (currentSpeed > 160) {
            currentSpeed = 160; 
        }
        update(); 
    }

    
    if (event->key() == Qt::Key_Down) {
        currentSpeed -= 2; 
        if (currentSpeed <= 0) {
            currentSpeed = 0; 
        }
        update(); 
    }
}


void CPUTempWidget::updateSpeed() {
    update();
}

CPUTempWidget::~CPUTempWidget() {}
