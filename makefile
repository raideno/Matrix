CC=g++
CPPFLAGS=-std=c++17

CPPFILES=util.cpp matrix.cpp sle.cpp index.cpp
CPPOBJECTS=util.o matrix.o sle.o index.o

BINARY=program

all:$(BINARY)

$(BINARY):$(CPPOBJECTS)
	$(CC) -o $@ $^
#	echo "compiled ($^) => $@"

%.o:%.c
	$(CC) $(CPPFLAGS) -c -o $@ $^
#	echo "compiled file $^"

run:$(BINARY)
	./$(BINARY)

clean:
	rm -rf $(BINARY) $(CPPOBJECTS)