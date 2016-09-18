# xu-gpiolib
Python GPIO library for Odroid-XU

### Compiling Python `gpio` library:

This part is pretty easy. In `lib/` run `build_lib.sh`. This will compile the `gpio.so` shared object file and move it to `lib/gpiolib/`.

### Odroid-XU Pin IO:

You can do pin IO with the following code in `pin_test/test.py`

```python
#!/usr/bin/env python
from ../lib/gpiolib import *


def main():
    sys_init()
    pinMode(27, OUTPUT)
    pinMode(24, INPUT)

    digitalWrite(27, ON)
    print(digitalRead(24))
    sys_clean()

if __name__ == '__main__':
    main()
```
You can only read/write to the GPIO/interrupt pins 13-27. Writing to even-numbered pins did not act predictably. However, reading in from even-numbered pins worked just fine.

The pinout is found [here](http://odroid.com/dokuwiki/doku.php?id=en:odroid-xu#expansion_connectors) and is laid out like this:

```
                         ^
                         |                        
                        CPU
==========================================================
29..27...................................................1
==========================================================
30..28...................................................2
==========================================================
```

Also, `pin_test/input_test.py` and `pin_test/output_test.py` do exactly what their names imply. They ask for a pin to test, and will read/write to itself.

### Running scripts on a button press:

Here's how you use `run_script/run.py`:

```
sudo chmod +x run.py
sudo chmod +x test.py
sudo ./run.py ./test.py arg1 arg2 arg3
```

where `arg1`... `argn` are arguments passed to `test.py`. `run.py` captures any output `test.py` passed to the console and creates a log file containing any output and date/time information in `logs/`.

`run.py` must be ran as root to do pin IO, however, it will change its ownership after pin IO is finished, so the logs it writes, as well as the process it spawns are both owned by the `odroid` user. Sadly, the script does not print either to the console or to the log file as it receives output from the subprocess; it prints everything out at once after the subprocess has finished executing.

### To run a script on startup as root:

Edit `/etc/rc.local` to run your script. Look up if anything special must be done to make it work with libraries.

### Example `run.py` log:

```
==========================================================
Running process '1 2 3' on pin 27 input.
Began: Sun Feb 14 00:03:17 2016
==========================================================
Process output:
==========================================================
test1
test2
test3
==========================================================
Took: 0.0507478713989 seconds to complete.
==========================================================
```
