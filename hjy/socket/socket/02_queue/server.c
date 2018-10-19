#include "common.h"

// 전역 변수
pthread_mutex_t mutx;
pthread_cond_t cond;

pthread_t recv_thr_list[MAX];
pthread_t accept_thr;
pthread_t select_thr;
pthread_t write_thr;

int server_sd;

TAILQ_HEAD(head_s, node) g_agent_head;	

node_t* node_init();						// node_t 생성 후, 반환
void queue_add(node_t *pNode, struct head_s *head);		// 넘겨준 node, insert
int queue_remove(node_t *pNode, struct head_s *head);		// 넘겨준 node, remove
int queue_count(struct head_s *head);				// queue conut 
int queue_get_max_fd(struct head_s *head);			// 가장 큰 fd값 리턴

int init_network();

void *thread_accept(void *arg);		// select를 사용한 accpet thread : 1개
void *thread_select(void *arg); 	// select
void *thread_recv(void *arg);		// recv thread * n개 

int main()
{
	int rc, i;
	
	// init TAILQ_HEAD
	TAILQ_INIT(&g_agent_head);
	pthread_mutex_init(&mutx, NULL);
	
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
	
	for(i=0; i<MAX; i++)
	{
		//pthread_create(&recv_thr_list[i], NULL, thread_recv, NULL);
	}

	pthread_create(&select_thr, NULL, thread_select, NULL);
	pthread_create(&accept_thr, NULL, thread_accept, NULL);
	
	// pthread_join
	for(i=0; i<MAX; i++)
	{
		//pthread_join(recv_thr_list[i], NULL);
	}	

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
				queue_count(&g_agent_head);
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

	int rc;
	char recvBuff[BUF_SIZE] = {0, };

	node_t *pNode = NULL;
		
	FD_ZERO(&recv_fds);
	int max_sd = 0;
	while(1)
	{
		// Agents FD_SET
		if(queue_count(&g_agent_head) != 0)
		{
			TAILQ_FOREACH(pNode, &g_agent_head, nodes)
			{
				if(pNode->used == false)
				{
					//printf("FD_SET(%d)\n", pNode->fd);
					pNode->used = true; 	// change status
					FD_SET(pNode->fd, &recv_fds);
					max_sd = queue_get_max_fd(&g_agent_head);
				}
			}
		}
		
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		memcpy(&temp_fds, &recv_fds, sizeof(recv_fds)); 	// backup
		//printf("\t(thread_select) wait max_sd(%d)\n",max_sd);
		//rc = select(max_sd+1, &temp_fds, 0, 0, NULL);

		rc = select(max_sd+1, &temp_fds, 0, 0, &timeout);
		//printf("\t(thread_select) recv_fd(%d)\n", recv_fd);
		if(-1 == rc)
		{	
			perror("thread_select");
		}else if(0 == rc)
		{
			continue;
		}

		//printf("\t(thread_select) recv_fd(%d)\n", rc);
		if(0 != queue_count(&g_agent_head))
		{
			TAILQ_FOREACH(pNode, &g_agent_head, nodes)
			{
				if(pNode->used = true)	// check status
				{
					if(FD_ISSET(pNode->fd, &temp_fds))
					{
						rc = recv(pNode->fd, recvBuff, BUF_SIZE, 0);
						if(-1 == rc)
						{
							printf("recv error(%d)\n",pNode->fd);
						}
						else if(0 == rc)
						{
							printf("\t(%d)disconnection\n", pNode->fd);
							FD_CLR(pNode->fd, &recv_fds);
							close(pNode->fd);
							queue_remove(pNode, &g_agent_head);
							
							if(0 == queue_count(&g_agent_head))
							{
								max_sd = server_sd;
							}else
							{
								max_sd = queue_get_max_fd(&g_agent_head);
							}
						}
						else
						{
							printf("\tmessage(%d) : %s\n",pNode->fd, recvBuff);
						}
					}
				}		
			}
		}
		memset(recvBuff, '\0', strlen(recvBuff));
		
	}	// while(1)
	
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
	fd_set fdread;
	char file_name[BUF_SIZE];
	
	struct timeval timeout;

	while(1)
	{
	}// while(1)


	printf("thread is ended : thread_recv\n");
	pthread_exit(NULL);
}


node_t* node_init()					// node_t 생성 후, 반환
{
	pthread_mutex_lock(&mutx);
	node_t *new_node = NULL;
	new_node = (node_t*)malloc(sizeof(node_t));
	pthread_mutex_unlock(&mutx);
	return new_node;
}
void queue_add(node_t *pNode, struct head_s *head)		// 넘겨준 node, insert
{
	pthread_mutex_lock(&mutx);
	TAILQ_INSERT_TAIL(head, pNode, nodes);
	pthread_mutex_unlock(&mutx);

	//printf("\tqueue_add() : %d\n", pNode->fd);
}

int queue_remove(node_t *pNode, struct head_s *head)	// 넘겨준 node, remove
{
	pthread_mutex_lock(&mutx);
	node_t *temp_node = NULL;

	TAILQ_FOREACH(temp_node, head, nodes)
	{
		if(temp_node == pNode)
		{
			printf("\tqueue_remove(%d)\n", temp_node->fd);
			TAILQ_REMOVE(head, temp_node, nodes);
			free(temp_node);
			temp_node = NULL;
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
