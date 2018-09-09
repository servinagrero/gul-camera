#ifndef GPIO_H
#define GPIO_H

#define OUTPUT "out"
#define INPUT  "in"


#include <string>
/* GPIO Class
 * Each object will control a GPIO on the raspberry pi
 * The pin number must be passed to the constructor
 */


class GPIO
{
public:
        GPIO(int pin = 4);
  
        int export_gpio(); // Exports GPIO
        int unexport_gpio(); // unexport GPIO
        int set_dir(std::string dir); // Set GPIO Direction
        int set_val(int val); // Set GPIO Value (putput pins)
        int get_val(); // Get GPIO Value (input/ output pins)
        int get_num(); // return the GPIO number associated with the instance of an object

private:
        int gpionum; //Gpio number used for the constructor
};

#endif // GPIO_H
