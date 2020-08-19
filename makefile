all: buildexample

buildexample: example.c mkstatlib
	gcc -std=c11 -o server example.c -L./ -lserver

mkstatlib: ./src/server.c
	gcc -std=c11 -c ./src/server.c ./src/crc.c ./src/list.c ./src/util.c
	ar r ./libserver.a server.o crc.o list.o util.o
	rm *.o

test:	./src/server.c example.c
	gcc -std=c11 -c -g -O0 ./src/server.c ./src/crc.c ./src/list.c ./src/list.c
	ar r ./libserver.a server.o crc.o list.o util.o
	rm *.o
	gcc -std=c11 -g -O0 -o server example.c -L./ -lserver
