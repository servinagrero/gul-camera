#include <fstream>
#include <iostream>
#include "GPIO.h"

GPIO::GPIO(int pin)
{
        this->gpionum = pin;
}

int GPIO::export_gpio()
{
        std::string export_str = "/sys/class/gpio/export";
        std::ofstream exportgpio(export_str.c_str());

        if ( ! exportgpio.is_open() )
        {
                std::cout << "Unable to export GPIO" << std::endl;

                return -1;
        }

        exportgpio << this->gpionum;
        exportgpio.close();
        return 0;
}

int GPIO::unexport_gpio()
{
        std::string unexport_str = "/sys/class/gpio/unexport";
        std::ofstream unexportgpio(unexport_str.c_str());

        if ( ! unexportgpio.is_open() )
        {
                std::cout << "Unable to unexport GPIO" << std::endl;

                return -1;
        }

        unexportgpio << this->gpionum;
        unexportgpio.close();
        return 0;
}


int GPIO::set_dir(std::string dir)
{
        std::string dir_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/direction";
        std::ofstream setdirgpio(dir_str.c_str());

        if ( ! setdirgpio.is_open() )
        {
                std::cout << "Unable to set direction of gpio" << std::endl;
                return -1;
        }

        setdirgpio << dir;
        setdirgpio.close();

        return 0;
}

int GPIO::set_val(int val)
{
        std::string val_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/value";
        std::ofstream setvalgpio(val_str.c_str());

        if ( ! setvalgpio.is_open() )
        {
                std::cout << "Unable to set gpio value" << std::endl;
                return -1;
        }

        setvalgpio << val;
        setvalgpio.close();

        return 0;
}

int GPIO::get_val()
{
        std::string num_str = "/sys/class/gpio/gpio" + std::to_string(this->gpionum) + "/value";
        std::ifstream numstr(num_str.c_str());

        if ( ! numstr.is_open() )
        {
                std::cout << "Unable to get gpio value" << std::endl;
                return -1;
        }

        int val;  
        numstr >> val;

        return val;
}

int GPIO::get_num()
{
        return this->gpionum;
}
