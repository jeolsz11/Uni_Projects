/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Simple TCP daytime client code in C
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <sys/socket.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 5012
#define MAX_LINE 29

int main(int argc, char *argv[])
{
    struct sockaddr_in serveraddr;
    char buf[MAX_LINE];
    char *host;
    struct hostent *hp;
    int s;
  
    /* "get" host's name from command line */
    if (argc == 2) {
        host = argv[1];
    } else {
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
    if (connect(s, (struct sockaddr *)&(serveraddr), sizeof(serveraddr)) < 0) {
        warnx("Connection error \n");
    }

    /* read in date from server; more specifically the socket */
    if ((recv(s, buf, MAX_LINE, 0)) == EOF) {
        errx(EXIT_FAILURE, "Output error");
    }
 
    /* print date time string to screen */
    puts(buf);
 
    close(s);

    return EXIT_SUCCESS;
}
