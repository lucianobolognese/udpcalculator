/*
 ============================================================================
 Name        : serverudp.c
 Author      : Luciano Domenico Bolognese
 Version     :
 Copyright   : Your copyright notice
 Description : SERVER UDP of a calculator which works in local address
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

// OPERATION FOR CALCULUS
int add(int a,int b) {
	int result;
	result=a+b;
	return result;
}

int sub(int a,int b) {
	int result;
	result=a-b;
	return result;
}

int mult(int a,int b) {
	int result;
	result=a*b;
	return result;
}

float division(int a,int b) {
	float ta=a;
	float tb=b;
	float result;
	result=ta/tb;
	return result;
}

// FUNCTION OF INTERACTION
int chat(struct sockaddr_in cad ,int sock){

	char var [512];
	char address [512];
	strcpy(address,ADDR);
	int recvMsgSize;


	msgStruct answer;
	memset(&answer, 0, sizeof(answer));





	while(1){
		memset(&var,0,sizeof(var));
		memset(&answer, 0, sizeof(answer));
		unsigned int cliAddrLen;
		cliAddrLen = sizeof(cad);

		recvMsgSize = recvfrom(sock, (char*) &answer , sizeof(answer), 0, (struct sockaddr*)&cad, &cliAddrLen);

		printf("Request operation '%s %d %d' ",answer.op,answer.a,answer.b);
		printf ("from client %s ", answer.hostname);
		printf("ip %s \n" ,inet_ntoa(cad.sin_addr));

		strcpy(var, answer.op);
		char op=answer.op[0];
		int c;

		switch(op)
		{
		case '+':
			c=add(answer.a,answer.b);
			sprintf(answer.result,"%d",c);
			break;

		case '/':
			if (answer.b != 0){
				float d;
				d=division(answer.a,answer.b);
				sprintf(answer.result,"%.2f",d);
			}
			else {
				strcpy(answer.result, "Math error");
			}
			break;
		case '*':
			c=mult(answer.a,answer.b);
			sprintf(answer.result,"%d",c);
			break;
		case '-':
			c=sub(answer.a,answer.b);
			sprintf(answer.result,"%d",c);
			break;

		case '=':
			strcpy(answer.result,"Disconnected");
			break;

		default:
			strcpy(answer.result, "Wrong operation");
			break;
		}
	unsigned int clAddrLen= sizeof(cad);
	sendto(sock, (char*) &answer , sizeof(answer), 0, (struct sockaddr *)&cad, &clAddrLen);

	if(op=='='){
		return 0;
	}
	}
}



int main(int argc, char *argv[]) {
	int port=PROTOPORT;

#if defined WIN32 // Winsock initialization
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);

	if (result != NO_ERROR) { //checking error in socket initialization
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	int sock;
	struct sockaddr_in sad;

	char echoBuffer[ECHOMAX];
	int recvMsgSize;

	// SOCKET'S CREATION
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	error("socket() failed");

	// CHECKING ERROR IN SOCKET'S CREATION

		if (sock < 0) {
			error ("Socket's creation failed \n");
			return -1;
		}

	// MAKING OF SERVER ADDRESS
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = PF_INET;
	sad.sin_port = htons(port);
	sad.sin_addr.s_addr = inet_addr("127.0.0.1" );


/*
	// SOCKET'S BIND
	if ((bind(sock, (struct sockaddr *) &sad, sizeof(sad))) < 0)
	error("bind() failed\n");
*/
	int n;
	n=bind(sock, (struct sockaddr*) &sad, sizeof(sad));
	if (n < 0)  {
		printf ("bind() failed %d \n", n);
		closesocket(sock);
		clearwinsock();
		return -1;
	} else {
		printf("Bind eseguita\n");
	}



	// INTERACTION WITH THE CLIENT
	while(1) {
		memset(&echoBuffer,0,sizeof(echoBuffer));
		struct sockaddr_in cad;
		printf("Server is online, waiting for an operation...\n");

		if(chat(cad, sock)==0){
					continue;
				}

	}

	// SOCKET CLOSING
	closesocket(sock);
	clearwinsock();
	system("pause");
	return EXIT_SUCCESS;
}

