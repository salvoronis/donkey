#include "./src/server.h"
#include <stdio.h>
#include <string.h>
#include "./src/map.h"

#define PORT 8081

char *html = "<!doctype html><html><head><meta charset=\"utf-8\"><title>the first donkey horseman</title></head><body><h1>You have C</h1><br><h3>You have a library for a castle & a library for the princess - CHARGE!</h3><br><h3>You rescue the princess her dog, her entrie wardrobe & everything she has ever eaten...Fuck - did I forget a NULL-terminator?</h3><img src=\"https://hsto.org/files/9cb/aa6/b5d/9cbaa6b5dc454f75ac9e88289665eb86.jpg\"/></body></html>";

void example(Request *req, Response *res){
	res->body=html;
}

void testfunc(char *anima, char *anime){
	printf("%s\n%s\n\n",anima,anime);
}

int main(){
	/*map *test = initMap();
	addToMap(test, "anime", testfunc, 5, sizeof(testfunc));
	void (*anime)(char*, char*);
	anime = getFromMap(test, "anime", 5);
	anime("holy shit", "anime");*/


	initServer(PORT, 10);
	addRout("/\0", example);
	listenServer();

	return 0;
}
