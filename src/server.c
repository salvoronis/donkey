#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "./crc.h"
#include <unistd.h>
#include "./server.h"
#include "./util.h"
#include "./map.h"

#define CHUNKS 50

int listenfd = 0;
map *routes;

struct Header {
	char *name;
	char *value;
};

struct Header stdHeadersResp[] = {
	{"Date", "Tue, 04 Aug 2020 02:03:02 GMT"},
	{"Server", "Donkey"},
	{"Last-Modified", "Tue, 02 Dec 2009 20:18:22 GMT"},
	{"Accept-Ranges", "bytes"},
	{"Content-Length", "1024"},
	{"Content-Type", "text/html"}
};

struct Response stdResp;

int cmpStr(void *a, void *b){
	return strcmp((char*) a, (char*)b);
}

int cmpHeader(void *a, void *b){
	char *tmp1 = (char *)a;
	struct Header *tmp2 = (struct Header *)b;
	int result = strcmp(tmp1,tmp2->name);
	return result;
}

void getHeaders(LinkedList *node, char *buffer){
	struct Header *head = (struct Header *)node->data;
	printf("anime -> %s end\n",head->value);
}

void respToStr(char *dest, struct Response resp){
	sprintf(dest, "%s %d %s\n", resp.proto, resp.statusCode, resp.statusDescription);
	struct Header *head;
	while (resp.headers != NULL) {
		head = (struct Header *)resp.headers->data;
		strcat(dest, head->name);
		strcat(dest, ": ");
		strcat(dest, head->value);
		strcat(dest, "\n");
		resp.headers = resp.headers->next;
	}
	strcat(dest, "\n");
	strcat(dest, resp.body);
}

void initServer(unsigned int port, unsigned int clientsNumb){
	signal(SIGINT, interrupt_signal);

	routes = initMap();

	struct sockaddr_in serv_addr;
	//make standart response
	LinkedList *node = NULL;
	for (int i = 0; i < 6; i++){
		push(&node, &stdHeadersResp[i], sizeof(stdHeadersResp[i]));
	}
	stdResp.statusDescription = "OK";
	stdResp.statusCode = 200;
	stdResp.proto = "HTTP/1.1";
	stdResp.headers = node;
	//stdResp.body = "donkey started breath";
	//end of standart response

	if ((listenfd = socket(AF_INET, SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(1);
	}
	int enable = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
	
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
		perror("bind");
		exit(2);
	}

	listen(listenfd, clientsNumb);

}

void requestParser(char *text, struct Request *req){
	char *prt;
	LinkedList *headers = NULL;

	prt = strtok(text, " \n");
	req->method = prt;

	prt = strtok(NULL, " \n");
	req->URL = prt;

	prt = strtok(NULL,":\n");
	req->proto = prt;

	prt = strtok(NULL,":\n");
	struct Header *header = (struct Header*)malloc(0);

	int headerSize = 0;
	while (prt){
		header = (struct Header*)realloc(header, (headerSize+1)*sizeof(struct Header));
		(header + headerSize)->name = prt;
		prt = strtok(NULL, "\n");
		if (prt == NULL)
			break;
		(header + headerSize)->value = prt+1;
		prt = strtok(NULL, ":");
		headerSize++;
	}
	for (int i = 0; i < headerSize; i++){
		push(&headers,(header+i), sizeof(struct Header));
	}
	req->headers = headers;
}

void listenServer(/*void (*example)(struct Request*, struct Response*)*/){
	int connfd = 0;
	//char *sendBuff = (char *)malloc(1024*sizeof(char));

	int reqSize = 0;

	//respToStr(sendBuff,stdResp);

	struct Request req;
	int bit = 0;

	while(1){
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		char *readBuff = (char *)malloc(1*sizeof(char));
		char *sendBuff = (char *)malloc(2048*sizeof(char));
		char *chunk = (char *)malloc(CHUNKS*sizeof(char));

		//respToStr(sendBuff, stdResp);
		//receiving data from client
		do{
			bit = recv(connfd, chunk, CHUNKS, 0);
			chunk = (char *)realloc(chunk,bit*sizeof(char));
			reqSize += bit;
			readBuff = (char *)realloc(readBuff, reqSize*sizeof(char));
			strcat(readBuff, chunk);
			memset(chunk,0,strlen(chunk));
		} while ( bit >= CHUNKS);
		bit = 0;
		free(chunk);
		//end of receive

		char *requestCopy = malloc(strlen(readBuff));
		strcpy(requestCopy, readBuff);
		requestParser(readBuff, &req);
		printf("%s\n",requestCopy);

		int contentLen = atoi(getHeaderByName("Content-Length", &req));
		if (contentLen != 0){
			char *body = malloc(contentLen * sizeof(char));
			body = requestCopy + (strlen(requestCopy) - contentLen);
			req.body = body;
		}
		//request completed!
		//put here user's func
		//example(&req,&stdResp);
		void (*handler)(Request*, Response*) = getFromMap(routes, req.URL, strlen(req.URL));
		handler(&req, &stdResp);
		respToStr(sendBuff,stdResp);

		write(connfd, sendBuff, strlen(sendBuff));
		
		free(readBuff);
		free(requestCopy);
		close(connfd);
	}
}

char *getHeaderByName(char *value, struct Request *req){
	LinkedList *headers = req->headers;
	LinkedList *headNode = getByNameNode(headers,value,cmpHeader);
	if (headNode == NULL)
		return "0";
	struct Header *header = (struct Header*)headNode->data;
	return header->value;
}

void addRout(char *URL, void (*handler)(Request*,Response*)){
	addToMap(routes, URL, handler, strlen(URL), sizeof(handler));
}
