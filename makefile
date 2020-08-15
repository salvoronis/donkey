all: buildexample

buildexample: example.c mkstatlib
	gcc -std=c11 -o server example.c -L./ -lserver

mkstatlib: ./src/server.c
	gcc -std=c11 -c ./src/server.c ./src/crc.c ./src/list.c
	ar r ./libserver.a server.o crc.o list.o
	rm *.o
