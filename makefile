SRC=./src/server.c ./src/crc.c ./src/list.c ./src/util.c
OBJ=server.o crc.o list.o util.o

all: buildexample

buildexample: example.c mkstatlib
	gcc -std=c11 -o server example.c -L./ -lserver

mkstatlib: ./src/server.c
	gcc -std=c11 -c $(SRC)
	ar r ./libserver.a $(OBJ)
	rm *.o

test:	./src/server.c example.c
	gcc -std=c11 -c -g -O0 $(SRC)
	ar r ./libserver.a $(OBJ)
	rm *.o
	gcc -std=c11 -g -O0 -o server example.c -L./ -lserver
