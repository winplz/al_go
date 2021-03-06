#include "common.h"

#define DEBUG_PRINT 0

// 전역 변수
pthread_mutex_t mutx;
pthread_cond_t cond;


pthread_mutex_t select_mutx;
pthread_cond_t select_cond;

pthread_mutex_t recv_mutx;
pthread_cond_t recv_cond;

pthread_t recv_thr_list[MAX];
pthread_t recv_thr;
pthread_t accept_thr;
pthread_t select_thr;
pthread_t write_thr;

int server_sd;
TAILQ_HEAD(head_s, node) g_agent_head, g_recv_head;


node_t* node_init();										// node_t 생성 후, 반환
void queue_add(node_t nodeValue, struct head_s *head);		// 넘겨준 node, insert
int queue_node_update(node_t *pNode, struct head_s *head);	// 넘겨준 node, change
int queue_remove(int fd, struct head_s *head);				// 넘겨준 node, remove
int queue_count(struct head_s *head);						// queue conut 
node_t* queue_get(struct head_s *head);						// 해당 head의 first 값을 리턴하고 삭제
int queue_get_max_fd(struct head_s *head);					// 가장 큰 fd값 리턴



void queue_print(struct head_s *head)
{
	printf("queue_print()\n");
	node_t *pNode = NULL;
	int i=1;
	TAILQ_FOREACH(pNode, head, nodes)
	{
		printf("%d_fd(%d)_status(%d)\n",i++, pNode->fd, pNode->status);
	}
	printf("end queue_print()\n");
}


int init_network();

void *thread_accept(void *arg);		// select를 사용한 accpet thread : 1개
void *thread_select(void *arg); 	// select
void *thread_recv(void *arg);		// recv thread * n개 


int main()
{
	int rc, i;
	
	// init TAILQ_HEAD
	TAILQ_INIT(&g_agent_head);
	TAILQ_INIT(&g_recv_head);
	
	pthread_mutex_init(&mutx, NULL);
	pthread_cond_init(&cond, NULL);
/*
	pthread_mutex_init(&select_mutx, NULL);
	pthread_cond_init(&select_cond, NULL);
*/
	pthread_mutex_init(&recv_mutx, NULL);
	pthread_cond_init(&recv_cond, NULL);

	// init_network( socket --> bind --> listen )
	if( -1 == (server_sd = init_network()))
	{
		printf("error : init_network()\n");
		return -1;
	}
	printf("\tsever_sd : %d\n",server_sd);

	// create thread
	for(i=0; i<RECV_THR_COUNT; i++)
	{
		pthread_create(&recv_thr_list[i], NULL, thread_recv, NULL);
	}
	pthread_create(&select_thr, NULL, thread_select, NULL);
	pthread_create(&accept_thr, NULL, thread_accept, NULL);
	
	// pthread_join
	for(i=0; i<RECV_THR_COUNT; i++)
	{
		pthread_join(recv_thr_list[i], NULL);
	}	
	pthread_join(select_thr, NULL);
	pthread_join(accept_thr, NULL);
	
	TAILQ_INIT(&g_agent_head);
	TAILQ_INIT(&g_recv_head);
	return 0;
}


int init_network()
{
	/*
	 * 1.create socket
	 * 2.set socket option
	 * 3.bind
	 * 4.listen
	 * 5.ready accept
	 */ 
	int optval = 1;
	struct sockaddr_in server_addr;
	
	// 1.
	if(-1 == (server_sd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("create socket");
		return -1;
	}

	// 2.
	// SO_REUSEDDR : 이미 사용된 주소를 재사용
	// ex) bind error : Address already in use 
	if(setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval)) < 0)
	{
		perror("setsocket");
		return -1;
	}
	
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT); // #define PORT 1234
	
	// 3.
	if(-1 == bind(server_sd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		perror("bind");
		return -1;
	}

	// 4.
	// backlog : 5
	if(-1 == listen(server_sd, 5))
	{
		perror("listen");
		return -1;
	}
	printf("----------서버 시작----------\n");
	printf("\t서버 포트 : %d\n", PORT);
	printf("\t연결 준비 완료\n");

	return server_sd;
}
void *thread_accept(void *arg)	// select를 사용한 accpet thread : 1개
{
	printf("Start : thread_accept\n");
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	int client_sd, client_len;
	struct sockaddr_in client_addr;
	fd_set recv_fds, temp_fds;
	int i, rc;

	FD_ZERO(&recv_fds);
	FD_SET(server_sd, &recv_fds);
				
	client_len = sizeof(client_addr);			
	node_t newNode;

	while(1)
	{
		memcpy(&temp_fds, &recv_fds, sizeof(recv_fds)); 	// backup
		rc = select(server_sd+1, &temp_fds, 0, 0, NULL);
		if(-1 == rc)
		{
			perror("thread_accept");
		}else if(0 == rc)
		{
			printf("select : timeout\n");
		}else
		{
			if(FD_ISSET(server_sd, &temp_fds))
			{		
				client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
				printf("\t연결(%d)\n", client_sd);
				
				// Add : agent_queue
				memset(&newNode, '\0', sizeof(newNode));
				newNode.fd = client_sd;
				newNode.status = SOCKET_READY;
				queue_add(newNode, &g_agent_head);
				/*					
				if(queue_count(&g_agent_head) == 1)
				{
					pthread_cond_signal(&select_cond);
				}
				*/
			}
		}
	}
	
	printf("thread is ended : thread_accept\n");
	pthread_exit(NULL);
}

void *thread_select(void *arg)
{
	printf("Start : thread_select\n");
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	fd_set recv_fds, temp_fds;
	struct timeval timeout;

	int rc, num, max_sd;
	node_t *pNode = NULL;
	node_t newNode;

	FD_ZERO(&recv_fds);
	max_sd = 0;
	while(1)
	{	
		num = queue_count(&g_agent_head);
		if(0 == num)
		{
#if DEBUG_PRINT
			printf("thread_select() Lock\n");
#endif
			/*
			pthread_mutex_lock(&select_mutx);
			pthread_cond_wait(&select_cond, &select_mutx);
			pthread_mutex_unlock(&select_mutx);
			*/
#if DEBUG_PRINT
			printf("thread_select() Unlock\n");
#endif 

		}else
		{
#if DEBUG_PRINT
			printf("thread_select num : %d\n", num);
			queue_print(&g_agent_head);
#endif
			// TAILQ_FOREACH is not safe during delete		
			pNode = TAILQ_FIRST(&g_agent_head);
			while(1)
			{
				if(pNode == NULL)
					break;
#if DEBUG_PRINT
				printf("---agent(%d)_status(%d)---\n", pNode->fd, pNode->status);	
#endif
				if(pNode->status == SOCKET_READY)
				{
					FD_SET(pNode->fd, &recv_fds);
					printf("FD_SET(%d)\n", pNode->fd);

					pNode->status = SOCKET_USED; 	// change status
					queue_node_update(pNode, &g_agent_head);
					max_sd = queue_get_max_fd(&g_agent_head);

				}else if(pNode->status == SOCKET_REMOVE)
				{
					FD_CLR(pNode->fd, &recv_fds);
					printf("FD_CLR(%d)\n", pNode->fd);
					close(pNode->fd);
					printf("연결 종료(%d)\n", pNode->fd);
					queue_remove(pNode->fd, &g_agent_head);
				}else if(pNode->status == SOCKET_USED)
				{
					// pass
				}else
				{
					printf("Exception!\n");
				}
				pNode = TAILQ_NEXT(pNode, nodes);
				if(pNode == NULL)
					break;
			}
			timeout.tv_sec = 0;
			timeout.tv_usec = 1000;
			memcpy(&temp_fds, &recv_fds, sizeof(recv_fds)); 	// backup
			//printf("\t(thread_select) wait max_sd(%d)\n",max_sd);
			rc = select(max_sd+1, &temp_fds, 0, 0, &timeout);
			//printf("\t(thread_select) select return value(%d)\n",rc);
			if(-1 == rc)
			{	
				perror("thread_select");
			}else if(0 == rc)
			{
				//printf("thread_select timeout\n");
			}else
			{
				TAILQ_FOREACH(pNode, &g_agent_head, nodes)
				{
					if(pNode->status == SOCKET_USED)	// check status
					{
						if(FD_ISSET(pNode->fd, &temp_fds))
						{
							// Add : recv_queue
							memset(&newNode, '\0', sizeof(newNode));
							newNode.fd = pNode->fd;
							newNode.status = pNode->status;
#if DEBUG_PRINT
							printf("FD : %d, STATUS : %d\n", newNode.fd, newNode.status);
							printf("\trecv_queue 넣어요 : %d\n", newNode.fd);
#endif
							queue_add(newNode, &g_recv_head);	
							pthread_cond_signal(&recv_cond);
							usleep(1000);
						}
					}		
				}
			}
		}
	
	}// while(1)
	
	printf("thread is ended : thread_select\n");
	pthread_exit(NULL);
}


void *thread_recv(void *arg)
{
	printf("Start : thread_recv(%lu)\n",pthread_self());

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	int rc, num, str_len;
	char msg[BUF_SIZE];
	node_t *pNode = NULL;
	
	struct timeval timeout;

	while(1)
	{
		num = queue_count(&g_recv_head);
		if(0 < num)
		{
			pNode = queue_get(&g_recv_head);	
			if(pNode != NULL)
			{
#if DEBUG_PRINT
				printf("\trecv_queue 꺼내요 : %d\n", pNode->fd);
#endif
				str_len = recv(pNode->fd, msg, BUF_SIZE, 0);
				if(0 == str_len)
				{
					// Change status
#if LOF_PRINT
					printf("\t(%d)socket status : REMOVE\n", pNode->fd);
#endif
					pNode->status = SOCKET_REMOVE;
					queue_node_update(pNode, &g_agent_head);

				}else if(-1 == str_len)
				{
					printf("\trecv : error\n", pNode->fd);
				}else
				{
					//printf("****************************\n");
					printf("msg(%d) : %s\n", pNode->fd, msg);
					//printf("****************************\n");
				}
				queue_remove(pNode->fd, &g_recv_head);
				memset(msg, '\0', strlen(msg));
			}else
			{

			}
		}else
		{
			pthread_mutex_lock(&recv_mutx);
			pthread_cond_wait(&recv_cond, &recv_mutx);
			pthread_mutex_unlock(&recv_mutx);
		}
	}// while(1)

	printf("thread is ended : thread_recv\n");
	pthread_exit(NULL);
}


node_t* node_init()					// node_t 생성 후, 반환
{
	pthread_mutex_lock(&mutx);
	node_t *new_node; 
	new_node = (node_t*)malloc(sizeof(node_t));
	new_node->fd = 0;
	new_node->status = SOCKET_INIT;
	pthread_mutex_unlock(&mutx);
	return new_node;
}
void queue_add(node_t nodeValue, struct head_s *head)		// 넘겨준 node, insert
{
	node_t *tempNode = node_init();
	pthread_mutex_lock(&mutx);
	tempNode->fd = nodeValue.fd;
	tempNode->status = nodeValue.status;
	TAILQ_INSERT_TAIL(head, tempNode, nodes);
	pthread_mutex_unlock(&mutx);
	//printf("\tqueue_add() : %d\n", tempNode->fd);
#if DEBUG_PRINT
	queue_print(head);
#endif
}

int queue_node_update(node_t *pNode, struct head_s *head)	// 넘겨준 node, change
{
	pthread_mutex_lock(&mutx);
	node_t *tempNode = NULL;
	TAILQ_FOREACH(tempNode, head, nodes)
	{
		if(tempNode->fd == pNode->fd)
		{
			tempNode->status = pNode->status;
			pthread_mutex_unlock(&mutx);
			//printf("queue_node_update() : success\n");
			return 1;
		}
	}
	pthread_mutex_unlock(&mutx);
	return -1;
}
int queue_remove(int fd, struct head_s *head)	// 넘겨준 node, remove
{
	node_t *pNode = NULL;
	pthread_mutex_lock(&mutx);

	// TAILQ_FOREACH is not safe
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
			pthread_mutex_unlock(&mutx);
			return 1;
		}
		pNode = TAILQ_NEXT(pNode, nodes);
		if(pNode == NULL)
			break;
	}
	printf("\t(Error)queue_remove(%d) : NOT FOUND\n", fd);
	pthread_mutex_unlock(&mutx);

	return -1;
}

int queue_count(struct head_s *head)
{
	int count = 0;
	node_t *pNode = NULL;
	pthread_mutex_lock(&mutx);
	
	TAILQ_FOREACH(pNode, head, nodes)
	{
		count++;
	}
#if DEBUG_PRINT
	//printf("queue_count(%d)\n", count);	
#endif
	pthread_mutex_unlock(&mutx);
	return count;
}
node_t* queue_get(struct head_s *head)
{
	node_t *pNode = NULL;
	pthread_mutex_lock(&mutx);
	
	if((pNode = TAILQ_FIRST(head)) == NULL)
	{
		printf("queue_get() is NULL\n");
	}

	pthread_mutex_unlock(&mutx);

	return pNode;
}

int queue_get_max_fd(struct head_s *head)			// 가장 큰 fd값 리턴
{
	int max = 0;
	node_t *pNode = NULL;
	pthread_mutex_lock(&mutx);

	TAILQ_FOREACH(pNode, head, nodes)
	{
		if(max < pNode->fd)
		{
			max = pNode->fd;
		}
	}
	pthread_mutex_unlock(&mutx);
	return max;

}
