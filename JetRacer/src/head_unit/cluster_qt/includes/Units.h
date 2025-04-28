#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <iostream>
#include <fstream>
#include <QWidget>

class Unit
{
    public:
        Unit() : unit(getLastValue()) {};
        ~Unit(){};
        Unit&  operator=(const Unit&) {return *this;}

        QString    getLastValue(void)
        {
            QString unit = "DPS";
            std::ifstream f("Unit.txt");
            if (!f.is_open())
            {
                std::cerr << "Error opening the file!";
                return unit;
            }
            std::string s;
            while (getline(f, s))
            {
                auto pos = s.find("unit = ");
                if (pos != std::string::npos)
                {
                    try
                    {
                        unit = s.substr(pos + 8);
                    }
                    catch (const std::exception& e)
                    {
                        unit = "DPS";
                       // std::cerr << "Something wrong with file, the result obtained is: " << s.substr(pos + 8) << "\n";
                    } 
                }
            }
            f.close();
            return unit;
        }
        QString unit;
        void    change(void)
        {
            std::ofstream f("Unit.txt");

            f << "unit = " << unit;
            f.close();
        };
};

#endif
