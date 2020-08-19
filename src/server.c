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

#define CHUNKS 50

int listenfd = 0;

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

void interrupt_signal(int sig){
	printf("%s","\n\ndonkey died. Sorry donkey\n");
	exit(0);
}

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
	stdResp.body = "donkey started breath";
	//end of standart response
	char buff[2048];
	respToStr(buff, stdResp);

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
		(header + headerSize)->value = prt;
		prt = strtok(NULL, ":");
		headerSize++;
	}
	for (int i = 0; i < headerSize; i++){
		push(&headers,(header+i), sizeof(struct Header));
	}
	req->headers = headers;
	
	//without that it doesn't work
	LinkedList *node = req->headers;
	struct Header *test;
	while (node != NULL) {
		test = (struct Header*)node->data;
		node = node->next;
	}
}

void listenServer(){
	int connfd = 0;
	char *sendBuff = (char *)malloc(1024*sizeof(char));
	//char *readBuff = (char *)malloc(1*sizeof(char));
	//char *chunk = (char *)malloc(CHUNKS*sizeof(char));

	int reqSize = 0;

	//memset(sendBuff, '0', sizeof(sendBuff));
	respToStr(sendBuff,stdResp);

	struct Request req;
	int bit = 0;

	while(1){
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		char *readBuff = (char *)malloc(1*sizeof(char));
		char *chunk = (char *)malloc(CHUNKS*sizeof(char));

		//respToStr(sendBuff, stdResp);
		do{
			bit = recv(connfd, chunk, CHUNKS, 0);
			chunk = (char *)realloc(chunk,bit*sizeof(char));
			reqSize += bit;
			readBuff = (char *)realloc(readBuff, reqSize*sizeof(char));
			strcat(readBuff, chunk);
			memset(chunk,0,strlen(chunk));
		} while ( bit >= CHUNKS);
		//chunk = (char *)realloc(chunk,CHUNKS*sizeof(char));

		//test -> passed! I'll use it later
		//func to parse http body but only if it's correct
		/*int contentLen = 21;
		char body[contentLen];
		memset(body,'0',strlen(body));
		for (int contentLenRev = 0; contentLenRev < contentLen; contentLenRev++) {
			body[contentLenRev] = readBuff[strlen(readBuff)-contentLen-1];
		}*/
		//test
		printf("%s\n",readBuff);
		requestParser(readBuff, &req);

		printf("value -> %s\n",getHeaderByName("Content-Lenght", &req));

		write(connfd, sendBuff, strlen(sendBuff));
		
		//readBuff = (char *)realloc(readBuff, 1);
		free(chunk);
		free(readBuff);
		//sendBuff = (char *)realloc(sendBuff, 1024 * sizeof(char));
		close(connfd);
	}
}

char *getHeaderByName(char *value, struct Request *req){
	LinkedList *headers = req->headers;
	LinkedList *headNode = getByNameNode(headers,value,cmpHeader);
	struct Header *header = (struct Header*)headNode->data;
	return header->value;
}
