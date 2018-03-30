#ifndef GPIO_H
#define GPIO_H

#include <string>
/* GPIO Class
 * Each object will control a GPIO on the raspberry pi
 * The pin number must be passed to the constructor
 */

using namespace std;


class GPIOClass
{
    public:
        GPIOClass();
        GPIOClass(string pin);

        int export_gpio(); // Exports GPIO
        int unexport_gpio(); // unexport GPIO
        int setdir_gpio(string dir); // Set GPIO Direction
        int setval_gpio(string val); // Set GPIO Value (putput pins)
        int getval_gpio(string& val); // Get GPIO Value (input/ output pins)
        string get_gpionum(); // return the GPIO number associated with the instance of an object

    private:
        string gpionum; //Gpio number used for the constructor
};

#endif