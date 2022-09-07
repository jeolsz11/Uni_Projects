/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Server side of the "Quote of the Day" system
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

#define SERVER_PORT 2017 //port 5012 used for testing
#define MAX_CHARACTERS 512
#define MAX_CONNECTS 5

void choose_quote(char *quote);
void cleanup_exit(int);

//'volatile' tells complier variable is NOT static like it seems to be
volatile sig_atomic_t shut_down; //global varible

int
main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;
    struct timeval tout;
    fd_set readFDS;
    char buf[MAX_CHARACTERS];
    int s; //socket
    int client_s; //client socket

    shut_down = false; //set variable

    /* build address data structure */
    memset(&server_addr, '\0', sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    /* create socket s */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) 
    {
        errx(EXIT_FAILURE, "SOCKET CREATION ERROR \n");
    }

    /* bind socket s */
    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        errx(EXIT_FAILURE, "SOCKET BIND ERROR \n");
    }

    /* set socket s to listen */
    if ((listen(s, MAX_CONNECTS)) == -1) 
    {
        errx(EXIT_FAILURE, "SOCKET LISTENING ERROR \n");
    }

    /* checking for SIGTERM; when SIGTERM caught call cleanup_exit */
    signal(SIGTERM, cleanup_exit);

    /* have socket s wait and accept connections until shut_down;
     * shut_down is triggered by SIGTERM signal */
    while (!shut_down) 
    {
        /* set read file descriptors to watch socket s to see when it has input */
        FD_ZERO(&readFDS);
        FD_SET(s, &readFDS);

        /* set timeout to wait 0.25 seconds */
        tout.tv_sec = 0;
        tout.tv_usec = 250000;

        /* check for signal every .25 seconds */
        client_s = select(s+1, &readFDS, NULL, NULL, &tout);

        if (client_s == -1)
        {
            errx(EXIT_FAILURE, "SIGNAL ISSUE \n");
        }

        if (client_s > 0) {
            choose_quote(buf);
            buf[strcspn(buf, "\n")] = '\0'; 
            
            /* send packet(buf) to client */
            send(client_s, buf, sizeof(buf), 0);
            close(client_s);
        }
    }

    close(s);

    return EXIT_SUCCESS;
}

void
choose_quote (char *quote)
{
    char* quote_bank[] = {"Respect is a rational process. - Dr. McCoy",
    "Change is the essential process of all existence. - Mr. Spock",
    "Genius doesn’t work on an assembly line basis. - Capt. Kirk",
    "Insufficient facts always invite danger. - Mr. Spock",
    "There is a way out of any cage. - Capt. Pike",
    "Vulcans never bluff. - Mr. Spock",
    "Peace was the way. - Capt. Kirk",
    "Not one hundred percent efficient, of course... but nothing ever is. - Capt. Kirk",
    "There are always alternatives. - Mr. Spock"
    };    
    
    int index = (rand() % (8 - 0 + 1)) + 0;
    //chooses random number bwteen 0 and 8, which are the index numbers of the quote_bank array
    
    quote = quote_bank[index];
}

void
cleanup_exit (int sig)
{
    //handle sigterm
    shut_down = true;
}
