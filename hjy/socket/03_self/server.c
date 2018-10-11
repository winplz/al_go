#include "common.h"

// 전역 변수
pthread_mutex_t mutx;
pthread_cond_t cond;

pthread_t recv_thr_list[MAX];
pthread_t recv_thr;
pthread_t accept_thr;
pthread_t select_thr;
pthread_t write_thr;

int server_sd;

TAILQ_HEAD(head_s, node) g_agent_head, g_recv_head, g_send_head;


node_t* node_init();									// node_t 생성 후, 반환
void queue_add(node_t *pNode, struct head_s *head);		// 넘겨준 node, insert
int queue_remove(node_t *pNode, struct head_s *head);	// 넘겨준 node, remove
int queue_count(struct head_s *head);					// queue conut 
int queue_get_max_fd(struct head_s *head);				// 가장 큰 fd값 리턴

void queue_print(struct head_s *head)
{
	node_t *pNode = NULL;
	TAILQ_FOREACH(pNode, head, nodes)
	{
		printf("\t(%d)\n",pNode->fd);
	}
}


int init_network();

void *thread_accept(void *arg);		// select를 사용한 accpet thread : 1개
void *thread_select(void *arg); 	// select
void *thread_recv(void *arg);		// recv thread * n개 


#define OPTION 0

int main()
{
	int rc, i;
	
	// init TAILQ_HEAD
	TAILQ_INIT(&g_agent_head);
	TAILQ_INIT(&g_recv_head);
	TAILQ_INIT(&g_send_head);
	
	pthread_mutex_init(&mutx, NULL);
	pthread_cond_init(&cond, NULL);


	// init_network( socket --> bind --> listen )
	if( -1 == (server_sd = init_network()))
	{
		printf("error : init_network()\n");
		return -1;
	}
	printf("\tsever_sd : %d\n",server_sd);

	// create thread
	// 1. recv_thr * MAX
	// 2. select_thr
	// 3. accept_thr 
#if OPTION	
	for(i=0; i<RECV_THR_COUNT; i++)
	{
		pthread_create(&recv_thr_list[i], NULL, thread_recv, NULL);
	}
#else
	pthread_create(&recv_thr, NULL, thread_recv, NULL);
#endif
	pthread_create(&select_thr, NULL, thread_select, NULL);
	pthread_create(&accept_thr, NULL, thread_accept, NULL);
	
	// pthread_join
#if OPTION
	for(i=0; i<RECV_THR_COUNT; i++)
	{
		pthread_join(recv_thr_list[i], NULL);
	}	
#else
	pthread_join(recv_thr, NULL);
#endif
	pthread_join(select_thr, NULL);
	pthread_join(accept_thr, NULL);
	
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
	printf("----------Start Server----------\n");
	printf("\tListenr on port : %d\n", PORT);
	printf("\tWaiting for connection\n");

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
				printf("\t(%d)connection\n", client_sd);

				node_t *new_node = node_init();
				new_node->fd = client_sd;
				new_node->used = false;
				queue_add(new_node, &g_agent_head);
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
	char recvBuff[BUF_SIZE] = {0, };

	node_t *pNode = NULL;
	
	while(1)
	{
		FD_ZERO(&recv_fds);
		max_sd = 0;
		if(0 != (num = queue_count(&g_agent_head) != 0))
		{
			TAILQ_FOREACH(pNode, &g_agent_head, nodes)
			{
				if(pNode->used == false)
				{
					printf("FD_SET(%d)\n", pNode->fd);
					pNode->used = true; 	// change status
					FD_SET(pNode->fd, &recv_fds);
					max_sd = queue_get_max_fd(&g_agent_head);
				}
			}


			timeout.tv_sec = 10;
			timeout.tv_usec = 10000;
			memcpy(&temp_fds, &recv_fds, sizeof(recv_fds)); 	// backup
			printf("\t(thread_select) wait max_sd(%d)\n",max_sd);
			rc = select(max_sd+1, &temp_fds, 0, 0, &timeout);
			if(-1 == rc)
			{	
				perror("thread_select");
			}else if(0 == rc)
			{
				continue;	
			}
			
			TAILQ_FOREACH(pNode, &g_agent_head, nodes)
			{
				if(pNode->used = true)	// check status
				{
					if(FD_ISSET(pNode->fd, &temp_fds))
					{
						printf("\tAdd on recv_head(%d)\n", pNode->fd);
						queue_add(pNode, &g_recv_head);	
						usleep(10000);
					}
				}		
			}
		
			
				//memset(recvBuff, '\0', strlen(recvBuff));
			
		}else	// if(queue_count(&g_agent_head) != 0)
		{
			printf("Empty Agent_queue\n");
			usleep(100000);
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

	int rc, num, idx, new_sd;
	char msg[BUF_SIZE];
	fd_set recv_fds, temp_fds;
	char file_name[BUF_SIZE];
	node_t *pNode = NULL;
	
	struct timeval timeout;

	FD_ZERO(&recv_fds);
	while(1)
	{
		if((num = queue_count(&g_recv_head)) > 0)
		{
			/*
			pNode = TAILQ_FIRST(&g_recv_head);
			printf("꺼냄(%d)\n", pNode->fd);
			TAILQ_REMOVE(&g_recv_head, pNode, nodes);
			usleep(100000);
			*/
			
			// 첫번째 값 얻은 후, 삭제
			pNode = TAILQ_FIRST(&g_recv_head);
			TAILQ_REMOVE(&g_recv_head, pNode, nodes);
			FD_SET(pNode->fd, &recv_fds);

			memcpy(&temp_fds, &recv_fds, sizeof(recv_fds));
			printf("wait select(%d)\n", pNode->fd);
			rc = select(pNode->fd+1, &temp_fds, 0, 0, NULL);
			if(-1 == rc)
			{
				perror("thread_recv");
			}else if(0 == rc)
			{
				printf("timeout\n");
			}else
			{
				printf("rc : %d\n", rc);	
			}
			
		}else
		{
			usleep(100000);
			/*
			printf("Lock : thread_recv()\n");
			pthread_mutex_lock(&mutx);
			pthread_cond_wait(&recv_cond, &mutx);
			pthread_mutex_unlock(&mutx);
			printf("Unlock : thread_recv()\n");
			*/
		}
	

	}// while(1)


	printf("thread is ended : thread_recv\n");
	pthread_exit(NULL);
}


node_t* node_init()					// node_t 생성 후, 반환
{
	pthread_mutex_lock(&mutx);
	node_t *new_node = NULL;
	new_node = (node_t*)malloc(sizeof(node_t));
	new_node->fd = 0;
	new_node->used = false;
	new_node->msg = (char *)malloc(sizeof(BUF_SIZE));
	pthread_mutex_unlock(&mutx);
	return new_node;
}
void queue_add(node_t *pNode, struct head_s *head)		// 넘겨준 node, insert
{
	pthread_mutex_lock(&mutx);
	TAILQ_INSERT_TAIL(head, pNode, nodes);
	pthread_mutex_unlock(&mutx);
	
	//printf("\tqueue_add() : %d\n", pNode->fd);
	//queue_print(head);
}

int queue_remove(node_t *pNode, struct head_s *head)	// 넘겨준 node, remove
{
	pthread_mutex_lock(&mutx);
	node_t *tempNode = NULL;

	TAILQ_FOREACH(tempNode, head, nodes)
	{
		if(tempNode == pNode)
		{
			printf("\tqueue_remove(%d)\n", tempNode->fd);
			TAILQ_REMOVE(head, tempNode, nodes);
			free(tempNode);
			tempNode = NULL;
			pthread_mutex_unlock(&mutx);
			return 1;
		}
	}
	printf("\t(Error)queue_remove : NOT FOUND\n");
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
	//printf("queue_count(%d)\n", count);	
	pthread_mutex_unlock(&mutx);
	return count;
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
