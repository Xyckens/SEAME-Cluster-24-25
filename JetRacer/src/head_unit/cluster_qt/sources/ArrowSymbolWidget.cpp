#include "../includes/ArrowSymbolWidget.h"

ArrowSymbolWidget::ArrowSymbolWidget(QWidget* parent, int x, int y, int width, int height)
    : QWidget(parent)
{
    color1 = Color();
    main_color = color1.main_color;
    left_color = main_color;
    right_color = main_color;
    accent_color = color1.accent_color;
    alphabet_color = color1.alphabet_color;

    int counter = color1.counter;
    setFocusPolicy(Qt::StrongFocus);
    setGeometry(x, y, width, height);
    angle = 240;
    step = 5;
    direction = "forward";
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ArrowSymbolWidget::variangle);
    timer->start(500); // 200 ms delay
    
    car = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/car.png");
    lane_img[0] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/lane_p.png");
    lane_img[1] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/lane_r.png");
    lane_img[2] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/lane_i.png");
    lane_img[3] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/lane_g.png");
    lane_alert = lane_img[counter];
    
    obj_prox_img[0] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/prox_p.png");
    obj_prox_img[1] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/prox_r.png");
    obj_prox_img[2] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/prox_i.png");
    obj_prox_img[3] = QPixmap("/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/prox_g.png");
    obj_prox_alert = obj_prox_img[counter];

    proximity = false;
    lane_off = false;
}

ArrowSymbolWidget::~ArrowSymbolWidget() {}


void ArrowSymbolWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2 - 50, height() / 4);

    int imageWidth = 100;
    int imageHeight = 150;
    if (!proximity)
        painter.drawPixmap(0, 0, imageWidth + 70, imageHeight + 20, obj_prox_alert);
    else if (!lane_off)
    {
        painter.drawPixmap(0, 0, imageWidth, imageHeight, car);
        if (direction == "forward")
           forwardArrows(painter);
        else
            backwardsArrows(painter);
    }
    else
    {
        painter.drawPixmap(0, 0, imageWidth, imageHeight, lane_alert);
        if (direction == "forward")
           forwardArrows(painter);
        else
            backwardsArrows(painter);
    }


}

void ArrowSymbolWidget::forwardArrows(QPainter& painter)
{
    float radius = 150;
    float smaller = radius / 2;
    float angle_y_offset_left = - angle * angle / 257.2 + 126;
    float angle_y_offset_right = angle * angle / 430 - 75.35;
    float curvature = angle * (M_PI / 180.0);
    float final_x = radius * std::sin(curvature);
    float final_y = radius * std::cos(curvature);
    float offset = 100;
    if (angle < 180)
        angle_y_offset_right = 0;
    else
        angle_y_offset_left = 0;

    QPainterPath path;
    path.moveTo(0, 0);

    QRectF controlRect(-radius, -radius, 2 * radius, 2 * radius);
    path.quadTo(QPointF(0, -smaller - angle_y_offset_left), QPointF(final_x, final_y - angle_y_offset_left));
    painter.setPen(QPen(left_color, 8));
    painter.drawPath(path);

    QPainterPath rightPath;
    rightPath.moveTo(offset, 0);
    rightPath.quadTo(QPointF(offset, -smaller - angle_y_offset_right), QPointF(offset + final_x, final_y - angle_y_offset_right));
    painter.setPen(QPen(right_color, 8));
    painter.drawPath(rightPath);
}

void ArrowSymbolWidget::backwardsArrows(QPainter& painter)
{
    float radius = 150;
    float smaller = radius / 2;
    float angle_y_offset_left = - angle * angle / 257.2 + 126;
    float angle_y_offset_right = angle * angle / 430 - 75.35;
    float offset_x = 100;
    float offset_y = 120;
    float curvature = angle * (M_PI / 180.0);
    float final_x = -1 * radius * std::sin(curvature);
    float final_y = -1 * radius * std::cos(curvature);
    if (angle < 180)
        angle_y_offset_right = 0;
    else
        angle_y_offset_left = 0;

    QPainterPath path;
    path.moveTo(0, offset_y);
    path.quadTo(QPointF(0, offset_y + smaller + angle_y_offset_right), QPointF(final_x, offset_y + final_y + angle_y_offset_right));
    painter.setPen(QPen(left_color, 8));
    painter.drawPath(path);

    QPainterPath rightPath;
    rightPath.moveTo(offset_x, offset_y);
    rightPath.quadTo(QPointF(offset_x, offset_y + smaller + angle_y_offset_left), QPointF(offset_x + final_x, offset_y + final_y + angle_y_offset_left));
    painter.setPen(QPen(right_color, 8));
    painter.drawPath(rightPath);
}

void   ArrowSymbolWidget::variangle(void)
{
    angle -= step;
    if (angle < 120 || angle > 240)
    {
        step *= -1;
        if (angle > 240 && direction == "forward")
            direction = "backwards";
        else if (angle > 240 && direction == "backwards")
            direction = "forward";
    }
    update();
}


void ArrowSymbolWidget::changeDirection(double ang, QString dir)
{
    if (angle == ang && direction == dir)
        return ;
    angle = ang;
    direction = dir;
    update();
}

void    ArrowSymbolWidget::changeColor(int  array_index)
{
    if (left_color == main_color)
        left_color = color1.main_color_array[array_index];
    if (right_color == main_color)
        right_color = color1.main_color_array[array_index];
    main_color = color1.main_color_array[array_index];
    accent_color = color1.accent_color_array[array_index];
    alphabet_color = color1.alphabet_color_array[array_index];
    lane_alert = lane_img[array_index];
    obj_prox_alert = obj_prox_img[array_index];
    update();
}

void    ArrowSymbolWidget::turnOnLanes(bool left, bool right)
{
    if (left)
        left_color = QColor(Qt::red);
    else
        left_color = main_color;
    if (right)
        right_color = QColor(Qt::red);
    else
        right_color = main_color;
    update();
}

void    ArrowSymbolWidget::proximityAlert(bool prox)
{
    if (proximity != prox)
    {
        proximity = prox;
        update();
    }
}