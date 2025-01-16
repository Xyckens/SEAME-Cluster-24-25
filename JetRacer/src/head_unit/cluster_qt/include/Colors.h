#ifndef COLOR_H
#define COLOR_H

#include <QWidget>
#include <string>

class Color
{
public:
    Color(){};
    ~Color(){};
    Color&          operator=(const Color&) {return *this;}
    int             counter = 0;

    QColor  main_color_array[4] = {"#312244", "#9A031E", "#FF99C8", "#9A031E"};
    QColor  main_color = main_color_array[0];

    QColor  accent_color_array[4] = {"#4D194D", "#FB8B24", "#A9DEF9", "#9A031E"};
    QColor  accent_color = accent_color_array[0];

    QColor  alphabet_color_array[4] = {"#006466", "#E36414", "#E4C1F9", "#9A031E"};
    QColor  alphabet_color = alphabet_color_array[0];

    QString background_array[4] = {"background-color: #212F45", "background-color: #5F0F40", "background-color: #D0F4DE", "background-color: #212F45"};
    QString background = background_array[0];
    void            indent()
    {
        this->counter++;
        if (this->counter > 3)
            this->counter = 0;
        this->main_color = main_color_array[counter];
        this->accent_color = accent_color_array[counter];
        this->alphabet_color = alphabet_color_array[counter];
        this->background = background_array[counter];
    };
    
};

#endif
