# slaches (/) in target names ? to target files in other directories ?

CC=g++
CPPFLAGS=-std=c++17

VPATH=$(wildcard lib/* build)

CPPFILES=util.cpp matrix.cpp math.cpp image.cpp sle.cpp window.cpp index.cpp
CPPOBJECTS=util.o matrix.o math.o image.o sle.o window.o index.o

BINARY=program

.PHONY: all run clean

all:$(BINARY)

$(BINARY):$(CPPOBJECTS)
	$(CC) -o $@ $^ -I/usr/include/SDL2 -lSDL2
	mv *.o build/

%.o:%.c
	$(CC) $(CPPFLAGS) -c -o $@ $^

run:$(BINARY)
	@./$(BINARY)

clean:
	echo "test"
	rm -rf $(CPPOBJECTS)
	rm -rf build/*