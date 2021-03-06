#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/queue.h>	//using : TAILQ 

#define PORT 4321
#define MAX 10 
#define RECV_THR_COUNT 10
#define BUF_SIZE 1024
#define SAFE_DELETE(p) { if((p) != NULL) { free (p); (p)=NULL; } }


typedef enum {false, true} BOOL;
#define SOCKET_INIT 0
#define SOCKET_READY 1
#define SOCKET_USED 2
#define SOCKET_REMOVE 3


typedef struct node{
	int fd;				// fd value
	int status;			// using status
	//char *msg;			// message content
	TAILQ_ENTRY(node) nodes;
}node_t;



