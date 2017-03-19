SRC = gpio.cpp test.cpp
OBJ = $(SRC:.cpp=.o)

CC = g++
LINK = g++

CFLAGS = -O3 -g -Wall -Wextra -Wpedantic -std=c++11
CXXFLAGS = $(CFLAGS)

TARGET = test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) -o $@ $^ $(CFLAGS)

.cpp:
	$(CC) -o $@ $@.cpp

clean:
	rm -rf *.o *.so *.gch *.orig $(TARGET)

format:
	astyle --indent=spaces --style=allman *.h *.cpp

install:
	g++ -fPIC -shared gpio.cpp -o libgpio.so
	sudo cp ./gpio.h /usr/include/gpio.h
	if [ -d "/usr/lib64" ]; then sudo cp ./libgpio.so /usr/lib64/libgpio.so; fi
	if [ -d "/usr/lib" ]; then sudo cp ./libgpio.so /usr/lib/libgpio.so; fi
