CC=g++
CPPFLAGS=-std=c++17

VPATH=lib build

CPPFILES=util.cpp matrix.cpp sle.cpp index.cpp
CPPOBJECTS=util.o matrix.o sle.o index.o

BINARY=program

.PHONY: all run clean

all:$(BINARY)

$(BINARY):$(CPPOBJECTS)
	$(CC) -o $@ $^
	mv *.o build/

%.o:%.c
	$(CC) $(CPPFLAGS) -c -o $@ $^

run:$(BINARY)
	@./$(BINARY)

clean:
	echo "test"
	rm -rf $(CPPOBJECTS)
	rm -rf build/*