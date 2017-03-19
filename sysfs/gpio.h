#ifndef Gpio_H_
#define Gpio_H_

#include <fstream>
#include <string>
#include <iostream>

#define HIGH true
#define LOW false
#define OUTPUT Output
#define INPUT Input

enum Mode {Output, Input};

class Gpio
{
public:
    Gpio();
    ~Gpio();
    void pinMode(int pin, Mode mode=OUTPUT);
    bool digitalRead(int pin);
    void digitalWrite(int pin, bool value=true);
    void pinOn(int pin);
    void pinOff(int pin);

private:
    std::ofstream unexport_file;
    std::ofstream export_file;

    const int num_pins = 42;

    // unique to the Odroid XU4 and I believe the XU3
    // pin_numbers[0] doesn't exists. I added it to make the indices line up.
    const int pin_numbers[42 + 1] = {-1, -1, -1, -1, 173, 174, 171, 192, 172, 191, 189, 190, -1,
                                     21, 210, 18, 209, 22, 19, 30, 28, 29, 31, -1, 25, 23, 24, 33,
                                     -1, -1, -1, -1, -1, -1, 187, 34, 188, 225, -1, 226, 229, 227,
                                     228};

    Mode pin_modes[42 + 1] = {OUTPUT};

    std::ofstream direction_files[42 + 1];
    std::ifstream input_files[42 + 1];
    std::ofstream output_files[42 + 1];
};

#endif /* end of include guard: Gpio_H_ */
