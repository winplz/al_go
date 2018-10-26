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

struct entry{
	node_t node;
	TAILQ_ENTRY(entry) entries;
};

typedef struct{
	pthread_mutex_t mutx;
	pthread_cond_t cond;
	TAILQ_HEAD(quehead, enyty) head;
}queue_t;




// agent_list
typedef struct {
	int fd;
	int status;
}agent_node_t;

typedef struct {
	agent_node_t list[MAX];
	int count;
	pthread_mutex_t mutx;
}agent_t;

agent_t g_agent;

void init_agentlist(agent_t *plist);
void destroy_agentlist(agent_t *plist);
int get_empty_idx(agent_t *plist);
int get_agent_idx_by_fd(agent_t *plist, int fd);
//agent_node_t *get_agent_info(agent_t *plist, int fd);
void add_agent(agent_t *plist, agent_node_t node);
void remove_agent(agent_t *plist, int fd);
void update_agent(agent_t *plist, agent_node_t node, int idx);

// recv_queue
queue_t g_recv_queue;



