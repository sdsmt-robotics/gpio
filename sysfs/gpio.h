#include <fstream>
#include <string>

#define HIGH true
#define LOW false
#define OUTPUT Output
#define INPUT Input

enum State {Output, Input};

class Gpio
{
public:
    Gpio();
    ~Gpio();
    void pinMode(int pin, State state);
    bool digitalRead(int pin);
    void digitalWrite(int pin, bool value);
    void pinOn(int pin);
    void pinOff(int pin);


private:
    std::ofstream importFiles;
    std::ofstream exportFiles;

    const int filesSize = 42;

    const int pinNumber[42] = {-1, -1, -1, 173, 174, 171, 192, 172, 191, 189, 190, -1, 21, 210, 18, 209, 22, 19, 30, 28, 29, 31, -1, 25, 23, 24, 33, -1, -1, -1, -1, -1, -1, 187, 34, 188, 225, -1, 226, 229, 227, 228};

    bool pins[42];

    std::ofstream directionFiles[42];
    std::ifstream valueFilesIn[42];
    std::ofstream valueFilesOut[42];
};


bool Gpio::digitalRead(int pin)
{
    int temp;
    valueFilesIn[pin - 1].seekg(0, std::ios::beg);
    valueFilesIn[pin - 1] >> temp;
    return (bool)temp;
}


void Gpio::digitalWrite(int pin, bool value)
{
    if (value)
        pinOn(pin);
    else
        pinOff(pin);
}


Gpio::Gpio()
{
    importFiles.open("/sys/class/gpio/export");
    exportFiles.open("/sys/class/gpio/unexport");
}


Gpio::~Gpio()
{
    for (int i = 0; i < filesSize; i++)
    {
        valueFilesOut[i].close();
        valueFilesIn[i].close();
        directionFiles[i].close();
        pins[i] = false;
    }

    exportFiles.close();
    importFiles.close();
}

void Gpio::pinOn(int pin)
{
    valueFilesOut[pin - 1] << "1" << std::endl;
}

void Gpio::pinOff(int pin)
{
    valueFilesOut[pin - 1] << "0" << std::endl;
}


//Create, open and sets the pen state
//State is if it is a input or output pin
//  true  = input pin
//  false = output pin

void Gpio::pinMode(int pin, State state)
{
    bool open = directionFiles[pin - 1].is_open();

    if (open)
    {
        if (pins[pin - 1])
        {
            if (state)
                return;
            pins[pin - 1] = state;
            valueFilesIn[pin - 1].close();
            valueFilesOut[pin - 1].open("/sys/class/gpio/gpio" + std::to_string(pinNumber[pin - 1]) + "/value");
            directionFiles[pin - 1] << "out" << std::endl;
        } else
        {
            if (!state)
                return;
            pins[pin - 1] = state;
            valueFilesOut[pin - 1].close();
            valueFilesIn[pin - 1].open("/sys/class/gpio/gpio" + std::to_string(pinNumber[pin - 1]) + "/value");
            directionFiles[pin - 1] << "in" << std::endl;
        }
    // create the pin
    } else
    {
        importFiles << pinNumber[pin - 1] << std::endl;
        std::string fileLocation = "/sys/class/gpio/gpio" + std::to_string(pinNumber[pin - 1]) + "/direction";
        directionFiles[pin - 1].open(fileLocation.c_str());
        pins[pin - 1] = state;
        if (state)
        {
            directionFiles[pin - 1] << "in" << std::endl;
            std::string fileLocation = "/sys/class/gpio/gpio" + std::to_string(pinNumber[pin - 1]) + "/value";
            valueFilesIn[pin - 1].open(fileLocation);
        } else
        {
            directionFiles[pin - 1] << "out" << std::endl;
            std::string fileLocation = "/sys/class/gpio/gpio" + std::to_string(pinNumber[pin - 1]) + "/value";
            valueFilesOut[pin - 1].open(fileLocation);
        }
    }
}
