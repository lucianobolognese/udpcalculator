/*
 * protocol.h client UDP
 *
 *  Created on: 4 nov 2021
 *      Author: Luciano Bolognese
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define ECHOMAX 512
#define PROTOPORT 56700
#define ADDRESS "srv.di.uniba.it"
#define ADDR "127.0.0.1"
#define QLEN 5
#define NO_ERROR 0


typedef struct {
	char op [ECHOMAX];
	int a;
	int b;
	char result [ECHOMAX];
} msgStruct;

typedef struct {
	const char *h_name; /* canonical name of host */
	char **h_aliases; /* alias list */
	short h_addrtype; /* host address type (AF_INET) */
	short h_length; /* length of address (INET)*/
	char **h_addr_list; /* A NULL-terminated list of
						addresses (in_addr) for the
						host in network byte order.*/
	#define h_addr h_addr_list[0];
} hostent;



#endif /* PROTOCOL_H_ */
