#include "gpio.h"
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
    Gpio g;

    g.pinMode(4, OUTPUT);

    while (true)
    {
        cout << "HIGH" << endl;
        g.pinOn(4);
        sleep(1);
        cout << "LOW" << endl;
        g.pinOff(4);
        sleep(1);
    }


    return 0;
}
