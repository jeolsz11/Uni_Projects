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

#include <ctype.h>
#include <err.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "protocol.h"

void cleanup_exit(int);
void upcase(char*);
void reverse(char*);

//'volatile' tells complier variable is NOT static like it seems to be
volatile sig_atomic_t shut_down; //global varible

int main(int argc, char *argv[]) {
	struct sockaddr_in serveraddr;
	struct timeval tout;
	fd_set readFDS;
	char buf[MAX_PACKET_SIZE];
	char payload[PAYLOAD_LEN+1];
	char opcode[OPCODE_LEN+1];
	int buf_len;
	int addr_len;
	int s;
	int new_s;
	int client_s;

	shut_down = false; //set variable

	/* build address data structure */
	memset(&serveraddr, '\0', sizeof serveraddr);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(SERVER_PORT);

	/* create socket s */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		errx(EXIT_FAILURE, "Socket creation error \n");
	}

	/* bind socket s */
	if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
		warnx("socket bind error \n");
	}

	/* set socket s to listen */
	if ((listen(s, SERVER_BACKLOG)) == -1) {
		errx(EXIT_FAILURE, "listening error");
	}

	/* checking for SIGTERM; when SIGTERM caught call cleanup_exit */
	signal(SIGTERM, cleanup_exit);

	/* have socket s wait and accept connections until shut_down;
	 * shut_down is triggered by SIGTERM signal */
	while (!shut_down) {
		/* set read file descriptors to watch socket s to see when it has input */
		FD_ZERO(&readFDS);
		FD_SET(s, &readFDS);

		/* set timeout to wait 0.25 seconds */
		tout.tv_sec = 0;
		tout.tv_usec = 250000;

		/* check for signal every .25 seconds */
		client_s = select(s+1, &readFDS, NULL, NULL, &tout);

		if (client_s == -1)
			warnx("select(2) issue");

		if (client_s > 0) {
			/* accept incoming packet(buf) */
			if ((new_s = accept(s, (struct sockaddr *)&serveraddr, &addr_len)) < 0) {
				errx(EXIT_FAILURE, "Accept error");
			}

			/* receive packet(buf) */
			memset(buf, '\0', sizeof buf);
			if ((recv(new_s, buf, sizeof buf, 0)) == EOF) {
			}

			/* deconstruct packet(buf) into parts */
			dissasemble_packet(buf, opcode, payload);
			//printf("packet disassembled: [%s]:[%s]\n", opcode, payload);

			/* check opcode for operations; then perform said operation */
			if (opcode[1] == 'P') {
				upcase(payload);
			}
			if (opcode[1] == 'E') {
				reverse(payload);
			}
			if (opcode[1] == 'V') {
				upcase(payload);
				reverse(payload);
			}

			/* rebuild packet(buf) */
			char reply[OPCODE_LEN+1] = "REPL";
			assemble_packet(reply, payload, buf);
			//printf("packet back to client: [%s]", buf);

			/* sanity check*/
			if ((check_packet(opcode, payload, buf))== 0) {
				errx(EXIT_FAILURE, "EROR Failed Sanity Check");
			}

			/* echo packet(buf) back to client */
			buf_len = strnlen(buf, sizeof(buf));
			send(new_s, buf, buf_len, 0);
			close(new_s);
		}
	}

	close(s);

	return EXIT_SUCCESS;
}

void cleanup_exit (int sig) {
	//handle sigterm
	shut_down = true;
}

