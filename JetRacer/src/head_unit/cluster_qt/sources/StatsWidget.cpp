#include "../includes/StatsWidget.h"
#include <iostream>


StatsWidget::StatsWidget(QWidget *parent, int x, int y, int width, int height)
    : QWidget(parent), width(width), height(height)
{
    setDistance(0);
    setAverage(0);
    setConsumed(0);
    setObstacles(0);
    color1 = Color();
    unit = "Km";
    main_color = color1.main_color;
    accent_color = color1.accent_color;
    alphabet_color = color1.alphabet_color;
    names << "Distance Traveled:" << "Average Speed:" << "Battery Consumed:" << "Obstacles Detected:";
    setGeometry(x, y, width, height);
}

#include <iostream>

void    StatsWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Set up font for text
    painter.setFont(QFont("Arial", 15, QFont::Bold));

    int spacing = 20;
    int rectWidth = width / 2 - spacing;
    int rectHeight = height / 2 - spacing - 50;

    // Draw two rows, each with two rectangles
    values.clear();
    values << distance << average << consumed << obstacles;
    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            int x = 10 + col * (rectWidth + spacing);
            int y = 10 + row * (rectHeight + spacing);

            // Draw rectangle
            painter.setPen(QPen(main_color, 8));
            painter.drawRect(x, y, rectWidth, rectHeight);

            painter.setPen(QPen(alphabet_color));
            painter.drawText(x + 20, y + 40, names[row * 2 + col]);
            painter.drawText(x + 20, y + 80, values[row * 2 + col]);
        }
    }
    QWidget::paintEvent(event);

}

void    StatsWidget::changeColor(int  array_index)
{
    main_color = color1.main_color_array[array_index];
    accent_color = color1.accent_color_array[array_index];
    alphabet_color = color1.alphabet_color_array[array_index];   
    update();
}

void    StatsWidget::setDistance(int dist)
{
    if (unit == "Mi")
        distance_int = dist * 0.6214;
    else
        distance_int = dist;
    distance = QString::number(distance_int) + " " + unit;
}

void    StatsWidget::setAverage(int avg)
{
    if (unit == "Mi")
        average_int = avg * 0.6214;
    else
        average_int = avg;
    average = QString::number(average_int) + " " + unit + " Per Hour";
}

void    StatsWidget::setConsumed(int con)
{
    consumed = QString::number(con) + "%";
}

void    StatsWidget::setObstacles(int obs)
{
    obstacles = QString::number(obs) + "";
}

void    StatsWidget::changeUnits(void)
{
    if (unit == "Km"){
        unit = "Mi";
        distance_int *= 0.6214;
        distance = QString::number(distance_int) + " " + unit;
        average_int *= 0.6214;
        average = QString::number(average_int) + " " + unit + " Per Hour";
    } else {
        unit = "Km";
        distance_int *= 1.609;
        distance = QString::number(distance_int) + " " + unit;
        average_int *= 1.609;
        average = QString::number(average_int) + " " + unit + " Per Hour";
    }
    update();
}
