#include "common.h"



int init_network()
{
	/*
	 * 1.create socket
	 * 2.set socket option
	 * 3.bind
	 * 4.listen
	 * 5.ready accept
	 */ 
	struct linger solinger = { 1, 0 };
	int optval = 1;
	int server_fd = 0;
	struct sockaddr_in server_addr;
	
	// 1.
	if(-1 == (server_fd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("create socket");
		return -1;
	}

	// 2.
	// SO_REUSEDDR : 이미 사용된 주소를 재사용
	// SO_LINGER : TIME_WAIT 해결을 위해
	// ex) bind error : Address already in use 
	// SO_LINGER, 특수한 경우가 아니면 사용하지 않는다
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval)) < 0)
	//if(setsockopt(server_fd, SOL_SOCKET, SO_LINGER, &solinger, sizeof(solinger)) == -1)
	{
		perror("setsocket");
		return -1;
	}
	
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT); // #define PORT 1234
	
	// 3.
	if(-1 == bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		perror("bind");
		return -1;
	}

	// 4.
	// backlog : 5
	if(-1 == listen(server_fd, 5))
	{
		perror("listen");
		return -1;
	}
	printf("----------서버 시작----------\n");
	printf("\t서버 포트 : %d\n", PORT);
	printf("\t연결 준비 완료\n");

	return server_fd;
}
/*
 * about : TAILQ functions
 */
node_t* node_init()					// node_t 생성 후, 반환
{
	node_t *new_node; 
	new_node = (node_t*)malloc(sizeof(node_t));
	new_node->fd = 0;
	return new_node;
}
void queue_add(node_t nodeValue, struct head_s *head)		// 넘겨준 node, insert
{
	node_t *tempNode = node_init();
	tempNode->fd = nodeValue.fd;
	TAILQ_INSERT_TAIL(head, tempNode, nodes);
#if DEBUG_PRINT
	printf("\tqueue_add() : %d\n", tempNode->fd);
#endif
}

int queue_remove(int fd, struct head_s *head)	// 넘겨준 node, remove
{
	node_t *pNode = NULL;
	pNode = TAILQ_FIRST(head);
	if(pNode != NULL)
	{
		TAILQ_REMOVE(head, pNode, nodes);
#if DEBUG_PRINT 
		printf("\tqueue_remove() : %d\n", pNode->fd);
#endif
		free(pNode);
	}else
	{
		printf("\t(Error)queue_remove(%d) : NOT FOUND\n", fd);
	}
	return 1;
	// TAILQ_FOREACH is not safe
	/*
	pNode = TAILQ_FIRST(head);
	while(1)
	{
		if(pNode == NULL)
			break;
		if(pNode->fd == fd)
		{
#if DEBUG_PRINT
			printf("\tqueue_remove() : %d\n", pNode->fd);
#endif
			TAILQ_REMOVE(head, pNode, nodes);
			free(pNode);
			return 1;
		}
		pNode = TAILQ_NEXT(pNode, nodes);
		if(pNode == NULL)
			break;
	}
	*/
	printf("\t(Error)queue_remove(%d) : NOT FOUND\n", fd);

	return -1;
}
int queue_isempty(struct head_s *head)						// queue empty yes(1) no(-1)
{
	int rc = -1;
	node_t *pNode = NULL;
	if((pNode = TAILQ_FIRST(head)) == NULL)
	{
		rc = 1;
	}
	return rc;
}

node_t* queue_get(struct head_s *head)
{
	node_t *pNode = NULL;
	
	if((pNode = TAILQ_FIRST(head)) == NULL)
	{
		//printf("queue_get() is NULL\n");
	}else
	{
		TAILQ_REMOVE(head, pNode, nodes);
		//printf("queue_get() --> %d\n", pNode->fd);
	}
	return pNode;
}


/*
 * about : agent_node_t / agent_t functions
 */

void init_agentlist(agent_t *plist)
{
	printf("init_agentlist()\n");
	// init() agent_list
	int i;
	plist->count = 0;
	plist->max_fd = 0;
	pthread_mutex_init(&plist->mutx, NULL);
	FD_ZERO(&plist->recv_fds);
	for(i=0; i<MAX; i++)
	{
		clear_agent_node(&plist->list[i]);
	}
}
void destroy_agentlist(agent_t *plist)
{
	printf("destroy_agentlist()\n");
	int i;
	plist->count = 0;
	pthread_mutex_destroy(&plist->mutx);
	FD_ZERO(&plist->recv_fds);
	for(i=0; i<MAX; i++)
	{
		clear_agent_node(&plist->list[i]);
	}
}

void clear_agent_node(agent_node_t *pnode)
{
	pnode->connTimes.start.tv_sec = 0;
	pnode->connTimes.start.tv_usec = 0;
	pnode->connTimes.end.tv_sec = 0;
	pnode->connTimes.end.tv_usec = 0;
	pnode->connTimes.average.tv_sec = 0;
	pnode->connTimes.average.tv_usec = 0;
	pnode->queueTimes.start.tv_sec = 0;
	pnode->queueTimes.start.tv_usec = 0;
	pnode->queueTimes.end.tv_sec = 0;
	pnode->queueTimes.end.tv_usec = 0;
	pnode->queueTimes.average.tv_sec = 0;
	pnode->queueTimes.average.tv_usec = 0;
	pnode->recvTimes.start.tv_sec = 0;
	pnode->recvTimes.start.tv_usec = 0;
	pnode->recvTimes.end.tv_sec = 0;
	pnode->recvTimes.end.tv_usec = 0;
	pnode->recvTimes.average.tv_sec = 0;
	pnode->recvTimes.average.tv_usec = 0;
	pnode->fd = -1;
	pnode->cnt = 0;
}

int get_empty_idx(agent_t *plist)
{
	int i, idx = -1;
	for(i=0; i<MAX; i++)
	{
		if(plist->list[i].fd == -1)
		{
			idx = i;
			break;
		}
	}
	return idx;
}
int get_agent_idx_by_fd(agent_t *plist, int fd)
{
	int i, count = 0, idx = -1;
	for(i=0; i<MAX; i++)
	{
		if(count == plist->count)
			break;
		if(plist->list[i].fd == -1)
			continue;
		if(plist->list[i].fd == fd)
		{
			idx = i;
			break;
		}
		count++;
	}
	return idx;
}
int get_agent_max_sd(agent_t *plist)
{
	int i, count = 0, max = 0;
	for(i=0; i<MAX; i++)
	{
		if(count == plist->count)
			break;
		if(-1 == plist->list[i].fd)
			continue;
		count++;
		if(max < plist->list[i].fd)
			max = plist->list[i].fd;
	}
	return max;
}
void agent_add(agent_t *plist, agent_node_t node)
{
	int i, idx = 0;
	idx = get_empty_idx(plist);

	if(idx == -1)
	{
		printf("agent_list is full\n");
		return ;
	}
	pthread_mutex_lock(&plist->mutx);

	if(plist->max_fd < node.fd)
		plist->max_fd = node.fd;
	plist->list[idx].fd = node.fd;
	// add_report
	add_report(&g_report, plist->list[idx]);

	plist->count++;
	FD_SET(plist->list[idx].fd, &plist->recv_fds);
	printf("conection client(%d)\n", plist->list[idx].fd);
	pthread_mutex_unlock(&plist->mutx);
}
void agent_remove(agent_t *plist, int fd)
{
	int i;

	for(i=0; i<MAX; i++)
	{
		if(plist->list[i].fd == fd)
		{
			pthread_mutex_lock(&plist->mutx);

			//printf("\tremove_agent(%d)\n", plist->list[i].fd);
			//printf("\tdisconnection(%d)\n", plist->list[i].fd);

			FD_CLR(plist->list[i].fd, &plist->recv_fds);
			close(plist->list[i].fd);
			
			clear_agent_node(&plist->list[i]);
			plist->count--;
			plist->max_fd = get_agent_max_sd(plist);
			pthread_mutex_unlock(&plist->mutx);
			break;
		}
	}
}
void agent_update(agent_t *plist, agent_node_t node)
{
	int idx = 0;
	pthread_mutex_lock(&plist->mutx);
	idx = get_agent_idx_by_fd(plist, node.fd);
	plist->list[idx].fd = node.fd;
	pthread_mutex_unlock(&plist->mutx);
}
void agent_count(agent_t *plist, int fd)
{
	int idx = 0;
	pthread_mutex_lock(&plist->mutx);
	idx = get_agent_idx_by_fd(plist, fd);
	plist->list[idx].cnt++;
	pthread_mutex_unlock(&plist->mutx);
}


long long timeval_diff(struct timeval *diff,
		struct timeval *end,
		struct timeval *start)
{
	struct timeval temp_diff;
	if(diff == NULL)
		diff = &temp_diff;
	
	diff->tv_sec = end->tv_sec - start->tv_sec;
	diff->tv_usec = end->tv_usec - start->tv_usec;
	while(diff->tv_usec < 0)
	{
		diff->tv_usec += 1000000;
		diff->tv_sec -= 1;
	}

	return (1000000LL * diff->tv_sec + diff->tv_usec);
}

void print_report(agent_t *plist)
{
	int i, total_cnt = 0;
	struct timeval conn_total, queue_total, recv_total;
	conn_total.tv_sec = 0;
	conn_total.tv_usec = 0;
	queue_total.tv_sec = 0;
	queue_total.tv_usec = 0;
	recv_total.tv_sec = 0;
	recv_total.tv_usec = 0;
	for(i=0; i<plist->count; i++)
	{
		conn_total.tv_sec += plist->list[i].connTimes.average.tv_sec;
		conn_total.tv_usec += plist->list[i].connTimes.average.tv_usec;
		queue_total.tv_sec += plist->list[i].queueTimes.average.tv_sec;
		queue_total.tv_usec += plist->list[i].queueTimes.average.tv_usec;
		recv_total.tv_sec += plist->list[i].recvTimes.average.tv_sec;
		recv_total.tv_usec += plist->list[i].recvTimes.average.tv_usec;
		total_cnt += plist->list[i].cnt;
	}
	
	printf("---------------------Report----------------------\n");
	printf("| Total Connection : %2d\n", plist->count);
	printf("| Total log recv count : %2d\n", total_cnt);
	printf("| Process(conn-disconn) average : %2ld sec %2ld usec\n", 
			(conn_total.tv_sec/plist->count), 
			(conn_total.tv_usec/plist->count));
	printf("| Process (queue_add~ get) average : %2ld sec %2ld usec\n", 
			(queue_total.tv_sec/plist->count), 
			(queue_total.tv_usec/plist->count));
	printf("| Process recv average : %2ld sec %2ld usec\n", 
			(recv_total.tv_sec/plist->count), 
			(recv_total.tv_usec/plist->count));
	printf("---------------------------------------\n");
	clear_list(plist);
}
void add_report(agent_t *plist, agent_node_t node)
{
	plist->list[plist->count++] = node;
}
void clear_list(agent_t *plist)
{
	int i;
	for(i=0; i<= plist->count; i++)
	{	
		clear_agent_node(&plist->list[i]);
	}
	plist->count = 0;
	plist->max_fd = 0;
}
void measureStartTime(int fd, int type)
{
	int idx = get_agent_idx_by_fd(&g_report, fd);

	if(type == 0)			// connTimes
	{
		gettimeofday(&g_report.list[idx].connTimes.start, NULL);
	}
	else if(type == 1)		// queueTimes
	{
		gettimeofday(&g_report.list[idx].queueTimes.start, NULL);
	}
	else if(type == 2) 		// recvTimes
	{
		gettimeofday(&g_report.list[idx].recvTimes.start, NULL);
	}
	else
	{
		printf("Error: mesureStartTime\n");
	}
}

void measureEndTime(int fd, int type)
{
	struct timeval diff;

	int idx = get_agent_idx_by_fd(&g_report, fd);
	if(type == 0) 	// connTimes
	{
		gettimeofday(&g_report.list[idx].connTimes.end, NULL);
		timeval_diff(&diff, &g_report.list[idx].connTimes.end, &g_report.list[idx].connTimes.start);

		g_report.list[idx].connTimes.average.tv_sec += diff.tv_sec;
		g_report.list[idx].connTimes.average.tv_usec += diff.tv_usec;

	}
	else if(type == 1) // queueTimes;
	{
		gettimeofday(&g_report.list[idx].queueTimes.end, NULL);
		timeval_diff(&diff, &g_report.list[idx].queueTimes.end, &g_report.list[idx].queueTimes.start);

		g_report.list[idx].queueTimes.average.tv_sec += diff.tv_sec;
		g_report.list[idx].queueTimes.average.tv_usec += diff.tv_usec;
	}
	else if(type == 2) // recvTimes
	{
		gettimeofday(&g_report.list[idx].recvTimes.end, NULL);
		timeval_diff(&diff, &g_report.list[idx].recvTimes.end, &g_report.list[idx].recvTimes.start);

		g_report.list[idx].recvTimes.average.tv_sec += diff.tv_sec;
		g_report.list[idx].recvTimes.average.tv_usec += diff.tv_usec;
	}
	else
	{
		printf("Error: mesureEndTime\n");
	}
}
