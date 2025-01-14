#ifndef BLINKERS_H
#define BLINKERS_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QTimer>

class Blinkers : public QWidget
{
    Q_OBJECT

public:
    explicit Blinkers(QWidget* parent = nullptr, std::string dir = "left", std::string mode = "off");
    virtual ~Blinkers();

    void    turnOnBlinkers(int blinker_key);
    bool    get_blinking(void);
    QTimer* getToggleTimer(void);
    bool    getIsImage1Visible(void);

private slots:
    void toggleImage(); // Slot to toggle the image

private:
    QLabel* imageLabel;         // QLabel to display the image
    QPixmap image1;             // First image
    QPixmap image2;             // Second image
    bool isImage1Visible;       // Boolean to track the current image
    QTimer* toggleTimer;        // Timer for toggling
    bool blinking;
};

#endif
