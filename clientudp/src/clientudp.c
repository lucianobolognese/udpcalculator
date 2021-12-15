/*
 ============================================================================
 Name        : clientudp.c
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

int main(int argc, char *argv[]) {
	char server_message[512]="Connection established\n";
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
		strcpy(request.hostname, address);
		token = strtok(NULL," ");
		strcpy(abs,token);
		port = atoi(abs);
		//strcpy(abs,token);
	}
	else if (argv[1]<0) {
		printf("Wrong address %s \n", argv[1]);
		return 0;
	}
	else {
		port=PROTOPORT;
		strcpy(address,ADDRESS);
		strcpy(str,address);
		strcpy(request.hostname,"localhost");
	}
	printf("Indirizzo: %s \n", address);
	printf("Porta: %d \n", port);

#if defined WIN32
// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif
	int sock;
	struct sockaddr_in sad;


	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	error("socket() failed");

	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = PF_INET;
	sad.sin_port = port;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");

	char input_string1 [512]; //first standard input string
	char input_string2 [512]; //second standard input string
	char input_string3 [512];

	hostent *servHost;
//	char servhost [ECHOMAX];


	servHost = gethostbyaddr((char *)&address, sizeof(address), AF_INET);

	//memset(&request, 0, sizeof(request));


	while(1){
		//memset(&request, 0, sizeof(request));
		//memset(&answer, 0, sizeof(answer));

		printf("Inserisci un'operazione (ex. + 31 12 or '=' per chiudere la calcolatrice): ");
		scanf("%s", &request.op);
		scanf("%d", &request.a);
		scanf("%d", &request.b);
		strcpy(request.result,"null");
		unsigned int servAddrLen= sizeof(sad);

/*
		if ((echoStringLen = sizeof(request)) > ECHOMAX){
			error("Echo word too long\n");
		}
*/

		sendto(sock, (char*) &request , sizeof(request), 0, (struct sockaddr*)&sad, sizeof(sad));

		// INVIO DELLA STRINGA ECHO AL SERVER
/*
		if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&sad, sizeof(sad)) != echoStringLen);
		error("sendto() sent different number of bytes than expected");
*/
		// RITORNO DELLA STRINGA ECHO

		recvfrom(sock,(char*) &answer , sizeof(answer), 0, (struct sockaddr*)&sad, &servAddrLen);
		printf("Ricevuto risultato dal server %s , ", answer.hostname);
		printf("ip %s : ", inet_ntoa(sad.sin_addr));
		printf("%s\n", answer.result);
	/*
		if (sad.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
	{
	fprintf(stderr, "Error: received a packet from unknown source.\n");
	exit(EXIT_FAILURE);
	}
	*/

	}


	closesocket(sock);
	clearwinsock();
	system("pause");
	return EXIT_SUCCESS;
}


