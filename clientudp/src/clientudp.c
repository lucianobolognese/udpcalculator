/*
 ============================================================================
 Name        : clientudp.c
 Author      : Luciano Domenico Bolognese
 Version     :
 Copyright   : Your copyright notice
 Description : CLIENT UDP of calculator which works in local address
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include <ctype.h>
#include <string.h>

void error(char *errorMessage) {
	printf("%s", errorMessage);
}

void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {

#if defined WIN32
// SOCKET'S INITIALIZATION
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif




	int port;
	char address [512];
	char str [512];
	char abs [512];
	msgStruct request;
	msgStruct answer;

	if (argv[1]>0) {
		strcpy(str,argv[1]);
		char * token = strtok(str, ":");
		strcpy(address,token);
		strcpy(request.servername, address);
		token = strtok(NULL," ");
		strcpy(abs,token);
		port = atoi(abs);

	}
	else if (argv[1]<0) {
		printf("Wrong address %s \n", argv[1]);
		return 0;
	}
	else {
		port=PROTOPORT;
		strcpy(address,ADDRESS);
		strcpy(str,address);
		strcpy(request.servername,"srv.di.uniba.it");
	}



	int sock;
	struct sockaddr_in sad;


	// SOCKET'S CREATION
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	error("socket() failed");

	// MAKING OF ADDRESS
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = PF_INET;
	sad.sin_port = htons(port);
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	char addr [512];
	strcpy(addr,inet_ntoa(sad.sin_addr));
	unsigned int servAddrLen= sizeof(sad);
	char var [ECHOMAX];
	char exit [ECHOMAX];
	strcpy(exit,"=");
	int recvMsgSize;


	// HOST STRUCT TO GET HOSTNAME
	struct hostent *host;
	const char* ip= "127.0.0.1";
	struct in_addr add;
	struct in_addr fromAddr;
	add.s_addr=inet_addr(ip);
	fromAddr.s_addr=inet_addr(ip);
	host = gethostbyaddr ((char *) &add, 4, AF_INET);
	char* canonical_name=host->h_name;
	strcpy(request.hostname,canonical_name);


	//INTERACTION WITH THE SERVER
	while(1){
		//memset(&request, 0, sizeof(request));
		//memset(&answer, 0, sizeof(answer));
		memset(&request.a,0,sizeof(request.a));
		memset(&request.b,0,sizeof(request.b));

		printf("Insert an operation (ex. + 31 12 or '=' to close the calculator): ");
		scanf("%s", &request.op);
		strcpy(var,request.op);
		if (strcmp(var,exit)==0) {
			sendto(sock, (char*) &request , sizeof(request), 0, (struct sockaddr*)&sad, sizeof(sad));
			recvfrom(sock,(char*) &answer , sizeof(answer), 0, (struct sockaddr*)&sad, &servAddrLen);

			printf("Received result from the server %s , ", answer.servername);
			printf("ip %s : ", inet_ntoa(sad.sin_addr));
			printf("%s\n", answer.result);
			system("pause");
			return 0;
		} else {

		scanf("%d", &request.a);
		scanf("%d", &request.b);
		strcpy(request.result,"null");

	// SENDING OF THE PACKET
		sendto(sock, (char*) &request , sizeof(request), 0, (struct sockaddr*)&sad, sizeof(sad));

		// RETURN OF THE ANSWER

		recvMsgSize=recvfrom(sock,(char*) &answer , sizeof(answer), 0, (struct sockaddr*)&sad, &servAddrLen);

		// CHECKING PACKET'S ADDRESS
		if (sad.sin_addr.s_addr != fromAddr.s_addr){
			fprintf(stderr, "Error: received a packet from another address.\n");
			return 0;
		}
		printf("Received answer from server %s , ", answer.servername);
		printf("ip %s : ", inet_ntoa(sad.sin_addr));
		printf("%s\n", answer.result);

		}
	}

	// SOCKET CLOSING
	closesocket(sock);
	clearwinsock();
	system("pause");
	return EXIT_SUCCESS;
}


