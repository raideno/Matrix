CC=g++
CPPFLAGS=-std=c++17

CPPFILES=util.cpp matrix.cpp sle.cpp 
CPPOBJECTS=util.o matrix.o sle.o 

BINARY=bin

all:$(BINARY)

$(BINARY):$(CPPOBJECTS)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) $(CPPFLAGS) -c -o $@ $^

clean:
	rm -rf $(BINARY) $(CPPOBJECTS)