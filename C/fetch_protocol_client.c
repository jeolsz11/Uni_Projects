/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Code starter for fetch protocol client
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "protocol.h"

static const size_t BLOCKSIZE = 512;	// 512 bytes
static const int TIMEOUT = 4;	        // 4 second timeout
static const int MAX_RETRIES = 3;       // maximum number of retries is 3

volatile sig_atomic_t transfer_complete;
volatile sig_atomic_t timeout_expired;	// timer expired

struct client_state
{
	FILE *              fd;
	char *              filename;
	int                 retries;  
	enum OPCODE         op_code;  
	unsigned short      block_id; 
	int                 sock;     // assumtion made this is the socket
	struct sockaddr_in  sadr;     // assumtion made this is the server address
};

static void send_request (struct client_state *, const char *);
static void transfer_data (struct client_state *);
static int check_socket (int);
static void process_datagram (struct client_state *);
static void sig_handler (int);
static void resend_ack (struct client_state *);

int main (int argc, char *argv[]) {
	struct client_state st;
	char *filename;
	struct hostent *hp;
	char *hostname;

	if (argc != 3) {
	  fprintf (stderr, "usage: %s hostname filename\n", getprogname ());
	  exit (EXIT_FAILURE);
	}

	/* "get" host's name from command line */
	if (argc == 2) {
		hostname = argv[1];
	} 
	else {
		errx(EXIT_FAILURE, "Could not find host \n");
	}

	/* translate host name into peer's IP address */
	hp = gethostbyname(hostname);
	if (hp == NULL) {
		errx(EXIT_FAILURE, "Could not translate host %s\n", hostname);
	}

	/* setup address structure (sadr) */
	memset(&sadr, '\0', sizeof sadr);
	sadr.sin_family = AF_INET;
	memcpy((char *)&sadr.sin_addr, hp->h_addr, hp->h_length);
	sadr.sin_port = htons(SERVER_PORT);

	/* create a SOCK_DGRAM socket */
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		errx(EXIT_FAILURE, "Socket creation error \n");
	}


	signal (SIGALRM, sig_handler);

	fd = fopen(filename, "w");

	if (fd != NULL) {
		send_request (st, filename);
		transfer_data (st);
		fclose(fd);
	}

	close(s);

	return EXIT_SUCCESS;
}

void send_request (struct client_state *xferstate, const char *filename) {
//-------------- Code Additions Begin --------------
    size_t getRequest;
    int sigCheck;
    char* buf;
    
    getRequest = mkGet(&buf, xferstate->filename);
    
    socklen_t addr_len = sizeof xferstate->sadr;
    sendto (xferstate->sock, buf, getRequest, 0,
	      (struct sockaddr *) &xferstate->sadr, addr_len);
    
    //start new timer 
    signal(SIGALRM, sig_handler);
    alarm(TIMEOUT);
    timeout_expired = false;
    xferstate->retries = 0;
    
    while (!timeout_expired) {
    	sigCheck = select(s+1, &rfds, NULL, NULL, &tv);
    	if (sigCheck > 0) {
            recvfrom();
            alarm(0);
    	}
    	else {
    	    xferstate->retries++;
    	}
    }
//-------------- Code Additions End --------------
}

void transfer_data (struct client_state *xferstate) {
  transfer_complete = false;

  while (!transfer_complete) {
      if (check_socket (xferstate->sock) > 0)
	    process_datagram (xferstate);

      if (timeout_expired)
	    resend_ack (xferstate);
    }

  alarm (0);
  timeout_expired = false;
  printf ("Done\n");
}

int check_socket (int fd) {
  fd_set rfds;
  struct timeval tv;

  FD_ZERO (&rfds);
  FD_SET (fd, &rfds);

  tv.tv_sec = 0;
  tv.tv_usec = 10000;

  return select (fd + 1, &rfds, NULL, NULL, &tv);
}

void process_datagram (struct client_state *xferstate) {
   char buf[OPCODE_LEN + SEQNUM_LEN + BLOCKSIZE]; //buffer
   socklen_t addr_len;
   size_t packet_len;
   
   alarm(0);
   timeout_expired = false;
   
   memset(buf, '\0', sizeof buf);
   addr_len = sizeof xferstate->sadr;
   packet_len = recvfrom(xferstate->sock, buf, sizeof buf, 0, 
           (struct sockaddr*)&xferstate->sadr, &addr_len);
   
   op_code = getOpCode(buf); 
   
   if (op_code == OP_GET) {
        if (block_id = expected) {
            len = mkDat(buf, xferstate->block_id, buf, packet_len);
            alarm(0);
            timeout_expired = false;
            transfer_complete = true
        }
        if (block_id = <expected) {
            len = mkAck(&buf, xferstate->block_id);
            alarm(0);
            timeout_expired = false;
        }
        if (block_id = >expected) {
            len = mkErr(buf, "ERROR");
        }
        sendto (xferstate->sock, buf, len, 0,
	                (struct sockaddr *) &xferstate->sadr, addr_len);
   }
   if (op_code == OP_ERR) {
      transfer_complete = true
   }
//-------------- Code Additions End --------------
}

void sig_handler (int sig) {
  timeout_expired = true;
}

void resend_ack (struct client_state *xferstate) {
  /* NOTE:
   * If this function is invoked the response timeout expired.
   * The last datagram will need to be resent and restart the timer
   * up to the max number of retries.
   * In the client it will either be a GET or an ACK datagram to resend.
   */
  assert (xferstate->op_code == OP_GET || xferstate->op_code == OP_ACK);

  alarm (0);
  timeout_expired = false;

  if (xferstate->retries < MAX_RETRIES) {
      socklen_t addr_len;
      char *packet;
      size_t reply_len;

      if (xferstate->op_code == OP_GET)
		  reply_len = mkGet (&packet, xferstate->filename);
      else
		  reply_len = mkAck (&packet, xferstate->block_id);

      addr_len = sizeof (struct sockaddr_in);
      sendto (xferstate->sock, packet, reply_len, 0,
	      (struct sockaddr *) &xferstate->sadr, addr_len);
      xferstate->retries++;
      free (packet);
      alarm (TIMEOUT);
    }
  else {
      printf ("\nTransfer timed out\n");
      transfer_complete = true;
    }
}
