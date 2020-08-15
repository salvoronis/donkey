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

int listenfd = 0;
int signal_inter = 0;

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
	signal_inter = 1;
}

int cmpStr(void *a, void *b){
	return strcmp((char*) a, (char*)b);
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
	char rest[strlen(text)];
	strcpy(rest,text);
	prt = strtok(rest, "\n");
	while (prt){
		//printf("-> %s|\n", prt);
		prt = strtok(NULL, "\n");
	}
	/*char *method = strtok(prt, " ");
	printf("that's my method -> %s\n", method);
	char *URL = strtok(NULL, " ");
	printf("that's my URL -> %s\n", URL);*/
}

void listenServer(){
	int connfd = 0;
	char sendBuff[1024];
	char *readBuff = (char *)malloc(1*sizeof(char));
	char *chunk = (char *)malloc(64*sizeof(char));

	int reqSize = 0;

	memset(sendBuff, '0', sizeof(sendBuff));

	struct Request req;
	int bit = 0;

	while(1){
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		respToStr(sendBuff, stdResp);
		while ((bit = recv(connfd, chunk, 64, 0)) >= 64){
			reqSize += bit;
			readBuff = (char *)realloc(readBuff, reqSize*sizeof(char));
			strcat(readBuff, chunk);
		}
		requestParser(readBuff, &req);
		write(connfd, sendBuff, strlen(sendBuff));

		close(connfd);
		if(signal_inter){
			printf("\ndonkey died. Sorry donkey\n");
			close(listenfd);
			exit(0);
		}
	}
}

//TODO request parser
//after that finally can start making handlers
