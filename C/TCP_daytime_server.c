/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Server side of string manipulation over TCP communication program
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#include <err.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SERVER_PORT 5012
#define MAX_CONNECTS 5
#define MAX_LINE 29

void cleanup_exit(int);

//'volatile' tells complier variable is NOT static like it seems to be
volatile sig_atomic_t shut_down; //global varible

int main(int argc, char *argv[]) {
	struct sockaddr_in serveraddr;
	struct timeval tout;
	fd_set readFDS;
	char buf[MAX_LINE];
	time_t current_time;
	int buf_len;
	int s;
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
	if ((listen(s, MAX_CONNECTS)) == -1) {
		errx(EXIT_FAILURE, "listening error");
	}

	/* checking for SIGTERM; when SIGTERM caught call cleanup_exit */
	signal(SIGTERM, cleanup_exit);

	/* have socket s wait and accept connecitons until shut_down; which is  triggered by SIGTERM */
	while (!shut_down) {
		/* set read file descriptors to watch socket s to see when it has input */
		FD_ZERO(&readFDS);
		FD_SET(s, &readFDS);
		
		/* set timeout to wait 0.25 seconds */
		tout.tv_sec = 0;
		tout.tv_usec = 250000;
		
		/* check for signal every .25 seconds */
		client_s = select(s+1, &readFDS, NULL, NULL, &tout);

		if (s == -1)
			warnx("select(2) issue");

		if (s > 0) {
			/* receive packet(buf); */
			current_time = time(NULL);
			buf_len = snprintf(buf, sizeof buf, "%.24s\r\n", ctime(&current_time));

			/* send packet(buf) back to client */
			send(client_s, buf, buf_len, 0);
			close(client_s);
		}
	}
	close(s);
	
    return EXIT_SUCCESS;
}

void cleanup_exit (int sig) {
    //handle sigterm
	shut_down = true;
}
