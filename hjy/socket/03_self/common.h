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
#include <signal.h> 	//using : SIGNAL


#define DEBUG_PRINT 0
#define PORT 4321
#define MAX 1024
#define RECV_THR_COUNT 10
#define BUF_SIZE 4
#define MAX_BUF_SIZE 1024
#define SAFE_DELETE(p) { if((p) != NULL) { free (p); (p)=NULL; } }


int init_network();



typedef enum {false, true} BOOL;

// TAILQ
typedef struct node{
	int fd;				// fd value
	//char *msg;			// message content
	TAILQ_ENTRY(node) nodes;
}node_t;

TAILQ_HEAD(head_s, node) g_recv_head;

node_t* node_init();										// node_t 생성 후, 반환
void queue_add(node_t nodeValue, struct head_s *head);		// 넘겨준 node, insert
int queue_remove(int fd, struct head_s *head);				// 넘겨준 node, remove
int queue_isempty(struct head_s *head);						// queue empty yes(1) no(-1)
node_t* queue_get(struct head_s *head);						// 해당 head의 first 값을 리턴하고 삭제

// struct about pthread
typedef struct{
	pthread_t thr;
	pthread_mutex_t mutx;
	pthread_cond_t cond;
	char name[255];
}thr_t;

thr_t g_recv_thr_list[RECV_THR_COUNT];
thr_t g_accept_thr;
thr_t g_select_thr;


// 시간 측정을 위한interval 함수
long long timeval_diff(struct timeval *diff,
		struct timeval *end,
		struct timeval *start);


// agent_list
typedef struct {
	struct timeval start;
	struct timeval end;
	struct timeval average;
}strtTime;

typedef struct {
	strtTime connTimes;		// 0 : conn ~ disconn 
	strtTime queueTimes;	// 1 : queue_add ~ queue_get
	strtTime recvTimes;		// 2 : before recv ~ after recv
	int fd;
	int cnt;
}agent_node_t;

typedef struct {
	agent_node_t list[MAX];
	int count;
	int max_fd;
	pthread_mutex_t mutx;
	fd_set recv_fds;
}agent_t;

agent_t g_agent, g_report;

void measureStartTime(int fd, int type);
void measureEndTime(int fd, int type);

void clear_list(agent_t *plist);
void clear_agent_node(agent_node_t *pnode);
void print_report(agent_t *plist);
void add_report(agent_t *plist, agent_node_t node);

void init_agentlist(agent_t *plist);
void destroy_agentlist(agent_t *plist);
int get_empty_idx(agent_t *plist);
int get_agent_idx_by_fd(agent_t *plist, int fd);
int get_agent_max_sd(agent_t *plist);
void agent_add(agent_t *plist, agent_node_t node);
void agent_remove(agent_t *plist, int fd);
void agent_update(agent_t *plist, agent_node_t node);
void agent_count(agent_t *plist, int fd);



