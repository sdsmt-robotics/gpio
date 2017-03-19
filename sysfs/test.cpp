#include "gpio.h"
#include <unistd.h>

int main() {
    Gpio g;

    g.pinMode(4, OUTPUT);

    while (true)
    {
        g.digitalWrite(4, HIGH);
        sleep(1);
        g.digitalWrite(4, LOW);
        sleep(1);
    }


    return 0;
}
