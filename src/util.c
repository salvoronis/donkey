#include <stdio.h>
#include <stdlib.h>

void interrupt_signal(int sig){
	printf("%s","\n\ndonkey died. Sorry donkey\n");
	exit(0);
}
