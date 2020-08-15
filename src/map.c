#include "crc.h"
#include <string.h>

static void (*router[0XFFFF])(void);

void addRout(unsigned char *path, void (*rout)(void)){
	unsigned short index = Crc16(path, strlen(path));
	router[index] = rout;
}
