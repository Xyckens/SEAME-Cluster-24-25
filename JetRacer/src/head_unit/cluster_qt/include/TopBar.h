#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>

class TopBar : public QWidget
{
    Q_OBJECT

public:
    explicit TopBar(QWidget* parent = nullptr);
    //explicit TopBar(TopBar *topbar);
    virtual ~TopBar();

    // Method to update the image at a specific index based on a condition
    bool setImageState(int index, bool state);

private:
    QHBoxLayout* layout;              // Layout to hold the images
    QVector<QLabel*> imageLabels;     // Vector to hold QLabel pointers for the images
    QVector<QPixmap> defaultImages;  // Vector to hold the default images
    QVector<QPixmap> altImages;      // Vector to hold the alternate images
};

#endif // TOPBAR_H
