#include "../includes/CameraWidget.h"

/**
 * @brief Constructs a CameraWidget with the specified parameters.
 *
 * This constructor initializes the CameraWidget, sets up the camera and video widget, and configures
 * the layout to display the camera feed. The widget is positioned and sized according to the provided
 * parameters.
 *
 * @param parent The parent widget.
 * @param x The x-coordinate of the widget's position.
 * @param y The y-coordinate of the widget's position.
 * @param width The width of the widget.
 * @param height The height of the widget.
 *
 * @note The camera feed is displayed using a QVideoWidget, which is added to the widget's layout.
 */
CameraWidget::CameraWidget(QWidget *parent, int x, int y, int width, int height)
    : QWidget(parent),
      camera(new QCamera(this)),
      videoWidget(new QVideoWidget(this)) {

    // Set up the capture session
    captureSession.setCamera(camera);
    captureSession.setVideoOutput(videoWidget);

    // Layout for the widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget); // Display the camera feed
    setLayout(layout);
    setGeometry(x, y, width, height);
}

/**
 * @brief Destroys the CameraWidget.
 *
 * This destructor cleans up dynamically allocated resources.
 */
CameraWidget::~CameraWidget() {
    delete camera;
}
