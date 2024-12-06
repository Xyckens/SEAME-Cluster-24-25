#ifndef CPUTTEMPWIDGET_H
#define CPUTTEMPWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>  // Include for key events

class CPUTempWidget : public QWidget {

    public:
        CPUTempWidget(QWidget* parent = nullptr);
        ~CPUTempWidget();
    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        int currentLevel;
        void drawScale(QPainter& painter, int centerX, int centerY, int radius);
        void drawNeedle(QPainter& painter, int centerX, int centerY, int radius);
        void drawCentralNumber(QPainter& painter, int centerX, int centerY);

    private slots:
        void updateLevel();
};

#endif // CPUTempWidget_H
