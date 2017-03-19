#include "gpio.h"

// TODO: redo with open, read, and write - abandon fstream
Gpio::Gpio()
{
    export_file.open("/sys/class/gpio/export");
    unexport_file.open("/sys/class/gpio/unexport");

    for (int i = 1; i < num_pins; i++)
    {
        int pin = pin_numbers[i];

        // if the Odroid actually supports that pin
        // http://odroid.com/dokuwiki/doku.php?id=en:xu4_hardware#expansion_connectors
        if (pin > 0)
        {
            // export each pin
            export_file << pin << std::endl;

            // open each direction file
            direction_files[i].open("/sys/class/gpio/gpio" + std::to_string(pin) + "/direction");
            // initialize each pin as OUTPUT pins
            direction_files[i] << "out" << std::endl;
        }
    }
}


Gpio::~Gpio()
{
    for (int i = 1; i < num_pins; i++)
    {
        int pin = pin_numbers[i];
        // if the pin exists
        if (pin > 0)
        {
            output_files[i].close();
            input_files[i].close();
            direction_files[i].close();
            pin_modes[i] = OUTPUT;

            // unexport each pin
            unexport_file << pin << std::endl;
        }
    }

    unexport_file.close();
    export_file.close();
}

bool Gpio::digitalRead(int pin)
{
    int temp;
    input_files[pin].seekg(0, std::ios::beg);
    input_files[pin] >> temp;
    return (bool)temp;
}


void Gpio::digitalWrite(int pin, bool value)
{
    if (value)
        pinOn(pin);
    else
        pinOff(pin);
}

void Gpio::pinOn(int pin)
{
    output_files[pin] << "1" << std::endl;
}

void Gpio::pinOff(int pin)
{
    output_files[pin] << "0" << std::endl;
}


//Create, open and sets the pen mode
//Mode is if it is a input or output pin
//  true  = input pin
//  false = output pin
void Gpio::pinMode(int pin, Mode mode)
{
    int pin_number = pin_numbers[pin];

    std::cout << "setting pin " << pin_number << " to mode " << mode << std::endl;

    if (pin_number < 0)
    {
        std::cerr << "Pin " << pin << " cannot be used. C.f. http://odroid.com/dokuwiki/doku.php?id=en:xu4_hardware#expansion_connectors" << std::endl;
        return;
    }
    if (mode == OUTPUT)
    {
        pin_modes[pin] = mode;
        input_files[pin].close();
        output_files[pin].open("/sys/class/gpio/gpio" + std::to_string(pin_numbers[pin]) + "/value");
        direction_files[pin] << "out" << std::endl;
    }
    else
    {
        pin_modes[pin] = mode;
        output_files[pin].close();
        input_files[pin].open("/sys/class/gpio/gpio" + std::to_string(pin_numbers[pin]) + "/value");
        direction_files[pin] << "in" << std::endl;
    }
}
