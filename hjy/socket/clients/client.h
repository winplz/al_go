// client.h


#include <stdio.h> 		//printf
#include <stdlib.h>
#include <string.h>    	//strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>	//inet_addr
#include <pthread.h>	// -lpthread

#define MAX_CLIENT_NUM 50
#define MSG_NUM_PER_SEC 100

#define MAX 50
#define BUF_SIZE 4
#define MAX_BUF_SIZE 1024
#define PORT 4321
#define HOST "localhost"

#define SOCKET int

typedef struct {
	int fd;
	int cnt;
}node_t;

typedef struct {
	node_t list[MAX];
	int count;
	int total;
	struct timeval average;
}report_t;

report_t g_report;

