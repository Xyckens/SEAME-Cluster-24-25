#include "../includes/FanSpeedWidget.h"

FanSpeedWidget::FanSpeedWidget(QWidget* parent, int x, int y, int width, int height)
    : QWidget(parent), current_speed(0)
{
    color1 = Color();
    index = color1.counter;
    main_color = color1.main_color;
    accent_color = color1.accent_color;
    alphabet_color = color1.alphabet_color;
    setFocusPolicy(Qt::StrongFocus);
    image_array[0] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fan_p.png");
    image_array[1] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fan_r.png");
    image_array[2] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fan_i.png");
    image_array[3] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fan_g.png");
    image = image_array[index];
    setGeometry(x, y, width, height);
}


void FanSpeedWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int center_x = (width() - 50) / 2;
    int center_y = (height() - 50) / 2;
    int radius = std::min((width() - 50), (height() - 50)) / 2 - 20;

    drawScale(painter, center_x, center_y, radius);

    drawNeedle(painter, center_x, center_y, radius);
    drawCentralNumber(painter, center_x, center_y);
}

void FanSpeedWidget::drawScale(QPainter& painter, int center_x, int center_y, int radius) {

    painter.setPen(QPen(main_color, 8));
    painter.drawEllipse(center_x - radius, center_y - radius, 2 * radius, 2 * radius);

}

void FanSpeedWidget::drawNeedle(QPainter& painter, int center_x, int center_y, int radius) {
    double startAngle = -45;
    double endAngle = 225;    

    double angle = startAngle + (endAngle - startAngle) * (double(current_speed) / 160);
    double rad = qDegreesToRadians(angle);
    int xStart = center_x - std::cos(rad) * (radius - 3);
    int yStart = center_y - std::sin(rad) * (radius - 3);
    int xEnd = center_x - std::cos(rad) * (radius + 1);
    int yEnd = center_y - std::sin(rad) * (radius + 1);
    painter.setPen(QPen(accent_color, 5));

    painter.drawLine(xStart, yStart, xEnd, yEnd);
}

void FanSpeedWidget::drawCentralNumber(QPainter& painter, int center_x, int center_y) {

    QFont font("Arial", 20, QFont::Bold); 
    painter.setFont(font);
    painter.setPen(QPen(alphabet_color));
    QString speedText = QString::number(current_speed);

    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(speedText);

    int x = center_x - textRect.width() / 2;
    int y = center_y + textRect.height() / 2 - 10;
    painter.drawText(x, y, speedText);

    QFont smallFont("Arial", 5, QFont::Bold); 
    painter.setFont(smallFont);

    QFontMetrics smallMetrics(smallFont);
    int kphWidth = smallMetrics.horizontalAdvance("RPM");
    int kphX = center_x - kphWidth / 2;
    int kphY = y + textRect.height() - 20; 

    painter.drawText(kphX, kphY, "RPM");
    image = image_array[index];
    if (!image.isNull()) {
        int imgWidth = 25; 
        int imgHeight = 25;
        int imgX = center_x - imgWidth / 2;
        int imgY = kphY + 20;
        painter.drawPixmap(imgX, imgY, imgWidth, imgHeight, image);
    }
}

void FanSpeedWidget::updateSpeed() {
    update();
}

void    FanSpeedWidget::changeColor(int  array_index)
{
    main_color = color1.main_color_array[array_index];
    accent_color = color1.accent_color_array[array_index];
    alphabet_color = color1.alphabet_color_array[array_index];
    index = array_index; 
    update();
}

FanSpeedWidget::~FanSpeedWidget() {}
