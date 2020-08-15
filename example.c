#include "./src/server.h"

#define PORT 8081

int main(){
	initServer(PORT, 10);
	listenServer();

	return 0;
}
