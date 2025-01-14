#include "../include/TopBar.h"

TopBar::TopBar(QWidget* parent) : QWidget(parent)
{
    
    layout = new QHBoxLayout(this);
    layout->setSpacing(1); 
    layout->setContentsMargins(0, 0, 0, 0); 
    QStringList defaultImagePaths = {
        "assets/icons/wheel.png",
        "assets/icons/brake.png",
        "assets/icons/abs.png", 
        "assets/icons/med.png",
        "assets/icons/max.png",
        "assets/icons/fog.png",
        "assets/icons/belt.png"
    };

    QStringList altImagePaths = {
        "assets/icons/wheel_on.png",
        "assets/icons/brake_on.png",
        "assets/icons/abs_on.png", 
        "assets/icons/med_on.png",
        "assets/icons/max_on.png",
        "assets/icons/fog_on.png",
        "assets/icons/belt_on.png"
    };

    
    for (const QString& path : defaultImagePaths) {
        QPixmap pixmap(path);
        if (pixmap.isNull()) {
            qWarning() << "Failed to load default image:" << path;
        }
        defaultImages.append(pixmap);
    }

    for (const QString& path : altImagePaths) {
        QPixmap pixmap(path);
        if (pixmap.isNull()) {
            qWarning() << "Failed to load alternate image:" << path;
        }
        altImages.append(pixmap);
    }

    
    for (int i = 0; i < 6; ++i) {
        QLabel* label = new QLabel(this);
        label->setFixedSize(40, 40); 
        //label->setScaledContents(true); // Ensure the image scales to fit the QLabel
        if (i < defaultImages.size()) {
            label->setPixmap(defaultImages[i]); // Set default image
        }
        imageLabels.append(label);
        layout->addWidget(label);
    }

    setLayout(layout);
    setMaximumWidth(250 * 2);
    setMinimumWidth(250 * 2);
}

TopBar::~TopBar() {}

bool    TopBar::setImageState(int index, bool state)
{
    if (index < 0 || index >= imageLabels.size()) {
        qWarning() << "Invalid index:" << index;
        return false;
    }

    // Set the image based on the state
    if (state) {
        imageLabels[index]->setPixmap(altImages[index]);
        return true;
    } else {
        imageLabels[index]->setPixmap(defaultImages[index]);
        return false;
    }
}
