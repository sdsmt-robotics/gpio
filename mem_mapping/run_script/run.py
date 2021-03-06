#!/usr/bin/env python
import subprocess
import sys
import pwd
import time
import os
import datetime
from gpiolib import *


def log(args, proc, now, pin):
    if not os.path.exists("logs"):
        os.makedirs("logs")
    filename = "logs/" + str(datetime.datetime.now()).split('.')[0] + ".log"
    with open(filename, 'w') as f:
        f.write("==================================================\n")
        f.write("Running process '" + ' '.join(args) + "' on pin " +
                str(pin) + " input.\n")
        f.write("Began: " + time.strftime("%c") + '\n')
        f.write("==================================================\n")
        f.write("Process output:\n")
        f.write("==================================================\n")
        for line in proc.stdout:
            sys.stdout.write(line)
            f.write(line)
            proc.wait()
        f.write("==================================================\n")
        f.write("Took: " + str(time.time() - now) + " seconds to complete.\n")
        f.write("==================================================\n")


def main(pin):
    """Runs command given from commandline arguments once pin `pin` is
    turned on.

    Example call: sudo ./run.py ./exec arg1 arg2
    """
    try:
        sys_init()
    except:
        print("Could not initialize. Must be run as root.")
        sys.exit()

    print "On pin", pin, "input, attempting to run: ", ' '.join(sys.argv[1:])
    pinMode(pin, INPUT)
    while True:
        if digitalRead(pin):
            # sys.argv is ["./run.py", "./test", "arg1", "arg2"]
            # [1:] strips off "./run.py" (first argument)
            print "Running", ' '.join(sys.argv[1:])
            print "Changing script ownership to non-root."
            uid = pwd.getpwnam('odroid')[2]
            os.setuid(uid)

            #now = time.time()
            subprocess.call(sys.argv[1:])

            print "==========================================================="
            # also prints STDOUT, STDERR to console
            #log(sys.argv[1:], proc, now, pin)
            print "==========================================================="
            sys_clean()
            print "Exiting"
            sys.exit()
        else:
            pass
        time.sleep(0.1)  # 0.1 of a second

if __name__ == '__main__':
    main(27)  # change this value to change the pin to monitor
