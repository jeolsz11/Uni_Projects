/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Client side of the "Quote of the Day" system
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

#define SERVER_PORT 2017 //port 5012 used for testing
#define MAX_CHARACTERS 512

int
main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;
    char buf[MAX_CHARACTERS+1];
    char *host;
    struct hostent *hp;
    int s; //socket

    /* "get" host's name from command line */
    if (argc == 2) 
    {
        host = argv[1];
    } 
    else
    {
        errx(EXIT_FAILURE, "COULD NOT FIND HOST \n");
    }

    /* translate host name into peer's IP address */
    hp = gethostbyname(host);
    if (hp == NULL)  
    {
        errx(EXIT_FAILURE, "COULD NOT FIND IP ADDRESS %s\n", host);
    }

    /* build address data structure */
    memset(&server_addr, '\0', sizeof server_addr);
    server_addr.sin_family = AF_INET;
    memcpy((char *)&server_addr.sin_addr, hp->h_addr, hp->h_length);
    server_addr.sin_port = htons(SERVER_PORT);

    /* create socket s */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
    {
        errx(EXIT_FAILURE, "CLIENT SOCKET CREATION ERROR \n");
    }

    /* connect to server */
    if (connect(s, (struct sockaddr *)&server_addr, sizeof server_addr) < 0)
    {
        errx(EXIT_FAILURE, "CLIENT CONNECTION ERROR \n");
    }

    /* receive and display packet(buf) from server */
    memset(buf, '\0', sizeof buf);
    recv(s, buf, sizeof buf, 0);
    puts(buf);

    close(s);

    return EXIT_SUCCESS;
}


