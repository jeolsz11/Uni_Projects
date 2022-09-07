/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Client side of string manipulation over TCP communication
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "protocol.h"

void display_menu (char *string, char *op);

int main(int argc, char *argv[]) {
	struct sockaddr_in serveraddr;
	char buf[MAX_PACKET_SIZE];
	char payload[PAYLOAD_LEN+1];
	char opcode[OPCODE_LEN+1];
	char *host;
	struct hostent *hp;
	int buf_len;
	int s;

	/* "get" host's name from command line */
	if (argc == 2) {
		host = argv[1];
	} 
	else {
		errx(EXIT_FAILURE, "Could not find host \n");
	}

	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (hp == NULL) {
		errx(EXIT_FAILURE, "Could not find IP address %s\n", host);
	}

	/* build address data structure */
	memset(&serveraddr, '\0', sizeof serveraddr);
	serveraddr.sin_family = AF_INET;
	memcpy((char *)&serveraddr.sin_addr, hp->h_addr, hp->h_length);
	serveraddr.sin_port = htons(SERVER_PORT);

	/* create socket s */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		errx(EXIT_FAILURE, "Socket creation error \n");
	}

	/* connect to server */
	if (connect(s, (struct sockaddr *)&serveraddr, sizeof serveraddr) < 0) {
		warnx("Connection error \n");
	}

	memset(opcode, '\0', OPCODE_LEN);
	memset(payload, '\0', PAYLOAD_LEN);

	/* get payload and opcode from user interaction with menu */
	display_menu(payload, opcode);
	payload[strcspn(payload, "\n")] = '\0';

	/* build packet(buf) */
	assemble_packet(opcode, payload, buf);

	/* sanity check */
	if ((check_packet(opcode, payload, buf))== 0) {
		errx(EXIT_FAILURE, "Failed Sanity Check");
	}

	/* send packet(buf) to server */
	buf_len = strnlen(buf, sizeof buf);
	send(s, buf, buf_len, 0);

	/* receive packet(buf) from server */
	memset(buf, '\0', sizeof buf);
	recv(s, buf, sizeof buf, 0);

	/* display packet(buf) from server to screen */
	puts(buf);

	close(s);

	return EXIT_SUCCESS;
}

void display_menu (char *string, char *op) {
	printf("Input string: \n");
	fgets(string, PAYLOAD_LEN, stdin);

	printf("NOOP - Nothing (no operation) : \n");
	printf("REVS - Reverse the string : \n");
	printf("UPCS - Uppercase the string : \n");
	printf("RVUC - Reverse and Uppercase the string : \n");
	printf("What should be done to string? (input one of the fours letter codes to choose): \n");
	fgets(op, OPCODE_LEN+1, stdin);
}
     
