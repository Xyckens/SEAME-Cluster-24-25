#ifndef BLINKERS_H
#define BLINKERS_H

#include "Colors.h"
#include <QLabel>
#include <QPixmap>
#include <QTimer>


/**
 * @brief The class represents a widget that simulates blinking indicators.
 * @details It uses a QTimer to toggle between two images, creating a blinking effect. 
 * The class supports turning the blinkers on/off and changing the color of the images.
 */

class Blinkers : public QWidget
{
        Q_OBJECT

    public:
        explicit Blinkers(QWidget* parent = nullptr, std::string dir = "left",
                          std::string mode = "off", int x = 0, int y = 0,
                          int width = 0, int height = 0);
        ~Blinkers() override;

        void    turnOnBlinkers(bool on_off);
        bool    getBlinking();
        void    changeColor(int array_index);
        QTimer* getToggleTimer(void);

    private slots:
        void    toggleImage();

    private:
        QLabel* image_label;
        QPixmap image1;
        QPixmap image1_array[4];
        QPixmap image2;
        QTimer* toggle_timer;
        bool    is_image1_visible;
        bool    blinking;
};

#endif
