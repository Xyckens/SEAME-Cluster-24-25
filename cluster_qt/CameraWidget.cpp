#include "includes/CameraWidget.h"

CameraWidget::CameraWidget(QWidget *parent)
    : QWidget(parent),
      camera(new QCamera(this)),
      videoWidget(new QVideoWidget(this)) {

    // Set up the capture session
    captureSession.setCamera(camera);
    captureSession.setVideoOutput(videoWidget);

    // Create start and stop buttons
    QPushButton *startButton = new QPushButton("Start Camera", this);
    QPushButton *stopButton = new QPushButton("Stop Camera", this);

    // Layout for the widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget); // Display the camera feed
    layout->addWidget(startButton);
    layout->addWidget(stopButton);
    setLayout(layout);

    // Connect buttons to camera controls
    connect(startButton, &QPushButton::clicked, camera, &QCamera::start);
    connect(stopButton, &QPushButton::clicked, camera, &QCamera::stop);
}

CameraWidget::~CameraWidget() {
    delete camera;
}
