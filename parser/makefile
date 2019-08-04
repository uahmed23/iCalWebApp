CFLAGS = gcc -Wall -std=c11 -Iinclude -fpic

all: list parser

list: src/LinkedListAPI.c
	$(CFLAGS) -shared ./src/LinkedListAPI.c -o ./bin/liblist.so

parser: src/CalendarParser.c src/HelperFunctions.c ./src/LinkedListAPI.c
	$(CFLAGS) -shared ./src/LinkedListAPI.c ./src/CalendarParser.c ./src/HelperFunctions.c -o ../libcal.so


clean:
	rm ./bin/*.o ./bin/*.so
