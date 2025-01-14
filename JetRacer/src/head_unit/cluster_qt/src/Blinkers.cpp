#include "../include/Blinkers.h"
#include <QHBoxLayout>

Blinkers::Blinkers(QWidget* parent, std::string dir, std::string mode)
    : QWidget(parent), isImage1Visible(true), blinking(false) // Initialize the first image as visible
{
    // Create a label to display the image
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    blinking = false;

    // Load two images to toggle
    if (dir == "left")
    {
        image1.load("/home/jegger/Documents/qt/SEAME-Cluster-24-25/JetRacer/src/head_unit/cluster_qt/assets/icons/left.png"); // Path to the first image
        image2.load("/home/jegger/Documents/qt/SEAME-Cluster-24-25/JetRacer/src/head_unit/cluster_qt/assets/icons/left_on.png"); // Path to the second image
    }
    else
    {
        image1.load("/home/jegger/Documents/qt/SEAME-Cluster-24-25/JetRacer/src/head_unit/cluster_qt/assets/icons/right.png"); // Path to the first image
        image2.load("/home/jegger/Documents/qt/SEAME-Cluster-24-25/JetRacer/src/head_unit/cluster_qt/assets/icons/right_on.png"); // Path to the second image
    }

    if (image1.isNull() || image2.isNull()) {
        qWarning() << "Failed to load images.";
    }

    // Set the initial image
    imageLabel->setPixmap(image1);

    // Create a QVBoxLayout to hold the QLabel
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);

    // Set the layout for this widget
    setLayout(layout);

    // Set up the QTimer to toggle the image every second
    // if (mode == "on")
    // {
    //     toggleTimer = new QTimer(this);
    //     connect(toggleTimer, &QTimer::timeout, this, &Blinkers::toggleImage);
    //     toggleTimer->start(1000); // 1000 ms = 1 second
    // }
}

Blinkers::~Blinkers()
{
    // Cleanup
    delete imageLabel;
    delete toggleTimer;
}

void Blinkers::toggleImage()
{
    // Toggle between the two images
    if (isImage1Visible) {
        imageLabel->setPixmap(image2);
    } else {
        imageLabel->setPixmap(image1);
    }

    // Flip the visibility flag
    isImage1Visible = !isImage1Visible;
}

void    Blinkers::turnOnBlinkers(int  on_off)
{
    if (on_off == true)
    {
        if (!blinking)
        {
            toggleTimer = new QTimer(this);
            connect(toggleTimer, &QTimer::timeout, this, &Blinkers::toggleImage);
            toggleTimer->start(500);
            blinking = true;
        }
    }
    else
    {
        if (toggleTimer)
            toggleTimer->stop();
        if (isImage1Visible)
            isImage1Visible = false;
        imageLabel->setPixmap(image1);
        blinking = false;
    }
}

bool    Blinkers::get_blinking()
{
    return blinking;
}

QTimer* Blinkers::getToggleTimer(void)
{
    if (toggleTimer)
        return toggleTimer;
    return nullptr;
}

bool    Blinkers::getIsImage1Visible(void)
{
    return isImage1Visible;
}
