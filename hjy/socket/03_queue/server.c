#include "common.h"

// 전역 변수

pthread_mutex_t mutx;
pthread_cond_t cond;

pthread_t recv_thr_list[MAX];
pthread_t accept_thr;
pthread_t select_thr;
pthread_t write_thr;


int server_sd;

TAILQ_HEAD(head_s, node) g_Head;


node_t* node_init();				// node_t 생성 후, 반환
void queue_add(node_t *pNode);		// 넘겨준 node, insert
int queue_remove(node_t *pNode);	// 넘겨준 node, remove


int init_network();

void *thread_accept(void *arg);		// select를 사용한 accpet thread : 1개
void *thread_select(void *arg); 	// select
void *thread_recv(void *arg);		// recv thread * n개 

int main()
{
	int rc, i;
	
	// init TAILQ_HEAD
	TAILQ_INIT(&g_Head);
	
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
		pthread_create(&recv_thr_list[i], NULL, thread_recv, NULL);
	}

	pthread_create(&select_thr, NULL, thread_select, NULL);
	pthread_create(&accept_thr, NULL, thread_accept, NULL);
	
	// pthread_join
	for(i=0; i<MAX; i++)
	{
		pthread_join(recv_thr_list[i], NULL);
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
	fd_set recv_fds;
	int i, rc;

	FD_ZERO(&recv_fds);
	FD_SET(server_sd, &recv_fds);
				
	client_len = sizeof(client_addr);
	

	
	while(1)
	{

		rc = select(server_sd+1, &recv_fds, 0, 0, NULL);
		if(rc == -1)
		{
			perror("select");
		}else if(rc == 0)
		{
			printf("select : timeout\n");
		}else
		{
			if(FD_ISSET(server_sd, &recv_fds))
			{		
				client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
				printf("\t(%d)connection\n", client_sd);
				node_t *new_node = node_init();
				new_node->fd = client_sd;
				queue_add(new_node);
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

	fd_set fdset, fdread;
	int max_sd, recv_fd;
	struct timeval timeout;

	int rc, i, idx;
	int loop_num;
	char recvBuff[BUF_SIZE] = {0, };

	/*
	 *	1. agent fdset 하기
	 *		1-1. agent_queue 확인
	 *		1-2. FDSET
	 */ 

	while(1)
	{
		// Agents FD_SET

		FD_ZERO(&fdread);
		max_sd = 0;

		if(max_sd == 0)
		{
		}
		else
		{
			memcpy(&fdset, &fdread, sizeof(fdread));

			timeout.tv_sec = 0;
			timeout.tv_usec = 10000;
			
			//printf("\t(thread_select) wait max_sd(%d)\n",max_sd);
			recv_fd = select(max_sd+1, &fdset, NULL, NULL, &timeout);
			//printf("\t(thread_select) recv_fd(%d)\n", recv_fd);
			if(recv_fd == -1)
			{	
				perror("recv_fd");
			}else if(recv_fd == 0)
			{
				continue;
			}else
			{
				//printf("\t(thread_select) recv_fd(%d)\n", recv_fd);
				for(i=0; i<MAX; i++)
				{		
					/*
					if(FD_ISSET(agent_get(i), &fdset))
					{
						if(agent_get(i) != 0)
						{

						}
					}
					*/
					memset(recvBuff, '\0', strlen(recvBuff));
				}
			}
		}
	}
	
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
	node_t *new_node = NULL;
	new_node = (node_t*)malloc(sizeof(node_t));
	return new_node;
}
void queue_add(node_t *pNode)		// 넘겨준 node, insert
{
	pthread_mutex_lock(&mutx);
	TAILQ_INSERT_TAIL(&g_Head, pNode, nodes);
	pthread_mutex_unlock(&mutx);

	printf("\tqueue_add()\n");
}
int queue_remove(node_t *pNode)		// 넘겨준 node, remove
{
	pthread_mutex_lock(&mutx);
	node_t *temp_node = NULL;

	TAILQ_FOREACH(temp_node, &g_Head, nodes)
	{
		if(temp_node == pNode)
		{
			printf("\tqueue_remove(%d)\n", temp_node->fd);
			TAILQ_REMOVE(&g_Head, temp_node, nodes);
			free(temp_node);
			temp_node = NULL;
			pthread_mutex_unlock(&mutx);
			return 1;
		}
	}
	printf("\tqueue_remove : NOT FOUND\n");
	pthread_mutex_unlock(&mutx);

	return -1;
}
