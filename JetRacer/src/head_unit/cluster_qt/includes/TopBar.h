#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include "Colors.h"
#include <iostream>


/**
 * @brief This class sets a Bar of icons in the Widget.
 * @details It updates the icons state and changes its colour.
 */
class TopBar : public QWidget
{
    Q_OBJECT

    public:
        TopBar(QWidget* parent = nullptr, int x = 0, int y = 0,
               int width = 0, int height = 0);
        ~TopBar() override;
    
        bool             setImageState(int index, bool state);
        void             changeColor(int  array_index);
    
    private:
        QHBoxLayout*     layout;
        QVector<QLabel*> image_labels;
        QVector<QPixmap> default_images;
        QVector<QPixmap> default_images_array[4];
        QVector<QPixmap> alt_images;
        QStringList      default_image_paths;
        QStringList      default_image_paths_array[4];
        QStringList      alt_image_paths;
};

#endif
