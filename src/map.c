#include "./crc.h"
#include <stdio.h>
#include <string.h>
#include "./map.h"
#include <stdlib.h>

map *initMap(int size){
	int cell = 0xFFFF;
	printf("%d\n",cell);
	map Map = malloc(cell*8);
	return Map;
}

void addToMap(map *dest, void *a, int size){
	unsigned int place = Crc16(a,size);
	*(place+dest) = malloc(size);
	*(place+dest) = a;
}

void *getFromMap(map *from, void *a, int size){
	unsigned short place = Crc16(a,size);
	printf("hoba2 -> %d\n",place);
	return *(from+place);
}
