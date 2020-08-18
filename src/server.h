#include "./list.h"
void initServer(unsigned int port, unsigned int clientsNumb);
extern int listenfd;
extern int signal_inter;
void listenServer(void);
typedef struct Response{
	char *statusDescription;
	int statusCode;
	char *proto;
	LinkedList *headers;
	char *body;
}Response;
typedef struct Request{
	char *method;
	char *URL;
	char *proto;
	LinkedList *headers;
}Request;
void getHeaderByName(char *dest, char *value, struct Request *req);
