#include "./crc.h"
#include <stdio.h>
#include <string.h>
#include "./map.h"
#include <stdlib.h>

map *initMap(){
	int cell = 0xFFFF;
	map Map = malloc(cell*8);
	return Map;
}

void addToMap(map *dest, void *key, void *value, int Ksize, int Vsize){
	unsigned int place = Crc16(key,Ksize);
	*(place+dest) = malloc(Vsize);
	*(place+dest) = value;
}

void *getFromMap(map *from, void *a, int size){
	unsigned short place = Crc16(a,size);
	return *(from+place);
}
