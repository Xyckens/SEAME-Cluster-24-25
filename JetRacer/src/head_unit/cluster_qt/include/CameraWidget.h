#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QPushButton>

class CameraWidget : public QWidget {
    Q_OBJECT

public:
    explicit CameraWidget(QWidget *parent = nullptr);
    virtual ~CameraWidget();

private:
    QCamera *camera;                       // Camera object
    QMediaCaptureSession captureSession;   // Capture session
    QVideoWidget *videoWidget;             // Video display widget
};


#endif // CAMERA_WIDGET_H
