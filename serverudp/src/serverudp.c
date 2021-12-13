/*
 ============================================================================
 Name        : serverudp.c
 Author      : luciano
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
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

int chat(struct sockaddr_in cad ,int sock){

	char var [512];
	int recvMsgSize;

	msgStruct answer;
	memset(&answer, 0, sizeof(answer));





	while(1){
		memset(&var,0,sizeof(var));
		memset(&answer, 0, sizeof(answer));
		unsigned int cliAddrLen;
		cliAddrLen = sizeof(cad);

		recvMsgSize = recvfrom(sock, (char*) &answer , sizeof(answer), 0, (struct sockaddr*)&cad, &cliAddrLen);

		printf("Richiesta operazione '%s %d %d' ",answer.op,answer.a,answer.b);
		printf ("dal client %s \n", inet_ntoa(cad.sin_addr));
			//		printf("ip \n", );

		strcpy(var, answer.op);
		char op=answer.op[0];
		int c;

		switch(op)
		{
		case '+':
			c=add(answer.a,answer.b);
			//itoa(c,result,10);
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
			break;

		default:
			strcpy(answer.result, "Wrong operation");
			break;
		}

	sendto(sock, (char*) &answer , sizeof(answer), 0, (struct sockaddr *)&cad, &cad);

	if(op=='='){
		printf("[+]Client disconnected.\n\n");
		return 0;
	}
	}
}



int main(int argc, char *argv[]) {
	char server_message[512]="Connection established\n";
	int port=PROTOPORT;

#if defined WIN32 // Winsock initialization
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);


	if (result != NO_ERROR) { //error checking in winsock initialization
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	int sock;
	struct sockaddr_in sad;

	char echoBuffer[ECHOMAX];
	int recvMsgSize;

	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	error("socket() failed");


	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&sad, 0, sizeof(sad));

	sad.sin_family = AF_INET;
	sad.sin_port = port;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");



	// BIND DELLA SOCKET
	if ((bind(sock, (struct sockaddr *)&sad, sizeof(sad))) < 0)
	error("bind() failed");



	// RICEZIONE DELLA STRINGA ECHO DAL CLIENT
	while(1) {
		memset(&echoBuffer,0,sizeof(echoBuffer));
		struct sockaddr_in cad;
		int c_socket;
		unsigned int cliAddrLen;
		printf("Server attivo, in attesa di un'operazione...\n");
		cliAddrLen = sizeof(cad);

		/*recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&cad, &cliAddrLen);
		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
		printf("Received: %s\n", echoBuffer);*/

		if(chat(cad, sock)==0){
					continue;
				}
		if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *)&cad,sizeof(cad)) != recvMsgSize)
			error("sendto() ha mandato un numero di byte diverso");




/*
	// RINVIA LA STRINGA ECHO AL CLIENT
	if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *)&cad,sizeof(cad)) != recvMsgSize)
	error("sendto() sent different number of bytes than expected");
	*/
	}
}

