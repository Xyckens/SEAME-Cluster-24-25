#include "../includes/TopBar.h"


/**
 * @brief Constructs a TopBar widget with the specified parameters.
 *
 * This constructor initializes the top bar widget, sets its geometry, and loads default and alternate
 * images for the icons. The widget displays a horizontal layout of icons, which can be toggled between
 * default and alternate states.
 *
 * @param parent The parent widget.
 * @param x The x-coordinate of the widget's position.
 * @param y The y-coordinate of the widget's position.
 * @param width The width of the widget.
 * @param height The height of the widget.
 */
TopBar::TopBar(QWidget* parent, int x, int y, int width, int height) : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(1);
    layout->setContentsMargins(0, 0, 0, 0);
    setGeometry(x, y, width, height);

    Color color = Color();
    int index = color.counter;
    default_image_paths_array[0] = {
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/wheel_p.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/brake_p.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/abs_p.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/med_p.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/max_p.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fog_p.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/belt_p.png"
    };
    default_image_paths_array[1] = {
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/wheel_r.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/brake_r.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/abs_r.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/med_r.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/max_r.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fog_r.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/belt_r.png"
    };
    default_image_paths_array[2] = {
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/wheel_i.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/brake_i.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/abs_i.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/med_i.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/max_i.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fog_i.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/belt_i.png"
    };
    default_image_paths_array[3] = {
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/wheel_g.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/brake_g.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/abs_g.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/med_g.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/max_g.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fog_g.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/belt_g.png"
    };
    alt_image_paths = {
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/wheel_on.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/brake_on.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/abs_on.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/med_on.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/max_on.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/fog_on.png",
        "/workspace/JetRacer/src/head_unit/cluster_qt/assets/icons/belt_on.png"
    };

    default_image_paths = default_image_paths_array[index];

    for (int j = 0; j < 4; j++)
    {
        for (const QString& path : default_image_paths_array[j]) {
            QPixmap pixmap(path);
            if (pixmap.isNull()) {
                qWarning() << "Failed to load default image:" << path;
            }
            default_images_array[j].append(pixmap);
        }
    }
    default_images = default_images_array[index];
    for (const QString& path : alt_image_paths) {
        QPixmap pixmap(path);
        if (pixmap.isNull()) {
            qWarning() << "Failed to load alternate image:" << path;
        }
        alt_images.append(pixmap);
    }


    for (int i = 0; i < 6; ++i) {
        QLabel* label = new QLabel(this);
        label->setFixedSize(40, 40);
    
        if (i < default_images.size()) {
            label->setPixmap(default_images[i]);
        }
        image_labels.append(label);
        layout->addWidget(label);
    }

    setLayout(layout);
    setMaximumWidth(250 * 2);
    setMinimumWidth(250 * 2);
}

/**
 * @brief Destroys the TopBar widget.
 *
 * This destructor cleans up any resources used by the widget.
 */
TopBar::~TopBar() {}

/**
 * @brief Sets the state of an image in the top bar.
 *
 * This method toggles the image at the specified index between its default and alternate states.
 *
 * @param index The index of the image to update.
 * @param state If true, the alternate image is displayed; if false, the default image is displayed.
 * @return true if the image was updated successfully, false if the index is invalid.
 */
bool    TopBar::setImageState(int index, bool state)
{
    if (index < 0 || index >= image_labels.size()) {
        qWarning() << "Invalid index:" << index;
        return false;
    }


    if (state) {
        image_labels[index]->setPixmap(alt_images[index]);
        return true;
    } else {
        image_labels[index]->setPixmap(default_images[index]);
        return false;
    }
}

/**
 * @brief Changes the color scheme of the top bar icons.
 *
 * This method updates the default images based on the provided array index, allowing the icons to
 * reflect a new color scheme.
 *
 * @param array_index The index of the color scheme.
 */
void    TopBar::changeColor(int  array_index)
{
    default_images = default_images_array[array_index];
    for (int i = 0; i < 6; ++i) {
        image_labels[i]->setPixmap(default_images[i]);
    }
}
