#include "./list.h"
void initServer(unsigned int port, unsigned int clientsNumb);
extern int listenfd;
extern int signal_inter;
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
	char *body;
	LinkedList *headers;
}Request;
void listenServer(/*void (*example)(struct Request*, struct Response*)*/);
char *getHeaderByName(char *value, struct Request *req);
void addRout(char *URL, void (*handler)(Request*, Response*));
