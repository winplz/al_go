#include "common.h"

// SIGNAL 처리
void init_signal();
void signal_handler(int signum);

// 전역 변수
int g_server_fd;

// recv_thr_list 전체를 풀어주기 위해서...
pthread_mutex_t recv_mutx;
pthread_cond_t recv_cond;

void init_threads();
void destroy_threads();

void *thread_accept(void *arg);		// select를 사용한 accpet thread : 1개
void *thread_select(void *arg); 	// select
void *thread_recv(void *arg);		// recv thread * n개 


int main()
{

	int rc, i;

	init_signal();

	// init TAILQ_HEAD
	TAILQ_INIT(&g_recv_head);
	
	pthread_mutex_init(&recv_mutx, NULL);
	pthread_cond_init(&recv_cond, NULL);

	// init : network( socket --> bind --> listen )
	if( -1 == (g_server_fd = init_network()))
	{
		printf("error : init_network()\n");
		return -1;
	}
	printf("\tsever_sd : %d\n",g_server_fd);
	printf("\tMAX_CLIENT_COUNT : %d\n", MAX);
	printf("\tRECV_THR_COUNT : %d\n", RECV_THR_COUNT);

	// init : agent_list
	init_agentlist(&g_agent);
	init_agentlist(&g_report);

	// init : thread
	init_threads();

	destroy_threads();
	TAILQ_INIT(&g_recv_head);
	destroy_agentlist(&g_agent);

	return 0;
}




void init_threads()
{
	int i;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);

	// g_recv_thr_list
	for(i=0; i<RECV_THR_COUNT; i++)
	{
		pthread_mutex_init(&g_recv_thr_list[i].mutx, &attr);
		pthread_cond_init(&g_recv_thr_list[i].cond, NULL);
		snprintf(g_recv_thr_list[i].name, sizeof(g_recv_thr_list[i].name), "RECV_THR%d",i);
		pthread_create(&g_recv_thr_list[i].thr, NULL, thread_recv, &g_recv_thr_list[i]);
		//pthread_create(&g_recv_thr_list[i].thr, NULL, thread_recv, (void *)&g_recv_thr_list[i]);
	}

	// g_select_thr
	pthread_mutex_init(&g_select_thr.mutx, &attr);
	pthread_cond_init(&g_select_thr.cond, NULL);
	snprintf(g_select_thr.name, sizeof(g_select_thr.name), "SELECT_THR");
	pthread_create(&g_select_thr.thr, NULL, thread_select, &g_select_thr);

	// g_accept_thr
	pthread_mutex_init(&g_accept_thr.mutx, &attr);
	pthread_cond_init(&g_accept_thr.cond, NULL);
	snprintf(g_accept_thr.name, sizeof(g_accept_thr.name), "ACCEPT_THR");
	pthread_create(&g_accept_thr.thr, NULL, thread_accept, &g_accept_thr);
}

void destroy_threads()
{
	int i;
	for(i=0; i<RECV_THR_COUNT; i++)
	{
		pthread_join(g_recv_thr_list[i].thr, NULL);
		pthread_mutex_destroy(&g_recv_thr_list[i].mutx);
		pthread_cond_destroy(&g_recv_thr_list[i].cond);
	}

	pthread_join(g_select_thr.thr, NULL);
	pthread_mutex_destroy(&g_select_thr.mutx);
	pthread_cond_destroy(&g_select_thr.cond);

	pthread_join(g_accept_thr.thr, NULL);
	pthread_mutex_destroy(&g_accept_thr.mutx);
	pthread_cond_destroy(&g_accept_thr.cond);
}


void *thread_accept(void *arg)	// select를 사용한 accpet thread : 1개
{
	thr_t *pThr;
	pThr = (thr_t *)arg;
	if(pThr == NULL)
	{
		printf("thread_accept is fail\n");
		pthread_exit(NULL);
	}
	else
		printf("START : %s\n", pThr->name);

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	int client_sd, client_len;
	struct sockaddr_in client_addr;
	fd_set recv_fds, temp_fds;
	int i, rc;

	FD_ZERO(&recv_fds);
	FD_SET(g_server_fd, &recv_fds);
				
	client_len = sizeof(client_addr);			

	while(1)
	{
		memcpy(&temp_fds, &recv_fds, sizeof(recv_fds)); 	// backup
		rc = select(g_server_fd+1, &temp_fds, 0, 0, NULL);
		if(-1 == rc)
		{
			perror("thread_accept");
		}else if(0 == rc)
		{
			printf("select : timeout\n");
		}else
		{
			if(FD_ISSET(g_server_fd, &temp_fds))
			{		
				pthread_mutex_lock(&pThr->mutx);
				client_sd = accept(g_server_fd, (struct sockaddr *)&client_addr, &client_len);
				// Add : agent_queue
				agent_node_t new_agent;
				new_agent.fd = client_sd;
				add_agent(&g_agent, new_agent);
				pthread_mutex_unlock(&pThr->mutx);

				if(g_agent.count == 1)
				{
					pthread_cond_signal(&g_select_thr.cond);
				}
			}
		}
	}
	
	printf("thread is ended : thread_accept\n");
	pthread_exit(NULL);
}

void *thread_select(void *arg)
{
	thr_t *pThr;
	pThr = (thr_t *)arg;
	if(pThr == NULL)
	{
		printf("thread_select is fail\n");
		pthread_exit(NULL);
	}
	else
		printf("START : %s\n", pThr->name);


	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	fd_set temp_fds;
	struct timeval timeout;

	int rc, i, count=0;
	node_t newNode;

	while(1)
	{
							
		pthread_mutex_lock(&pThr->mutx);
		count = g_agent.count;
		pthread_mutex_unlock(&pThr->mutx);

		if(0 != count)
		{
			timeout.tv_sec = 0;
			timeout.tv_usec = 1000;
			memcpy(&temp_fds, &g_agent.recv_fds, sizeof(g_agent.recv_fds)); 	// backup
			rc = select(g_agent.max_fd+1, &temp_fds, 0, 0, &timeout);
			printf("\t(thread_select) select return value(%d)\n",rc);
			if(-1 == rc)
			{	
				perror("thread_select");
			}else if(0 == rc)
			{
				//printf("thread_select timeout\n");
			}else
			{
				for(i=0; i<MAX; i++)
				{
					if(-1 == g_agent.list[i].fd)
						continue;
					else
					{
						if(FD_ISSET(g_agent.list[i].fd, &temp_fds))
						{
							// Add : recv_queue
							//pthread_mutex_lock(&pThr->mutx);
							memset(&newNode, '\0', sizeof(newNode));
							newNode.fd = g_agent.list[i].fd;
							queue_add(newNode, &g_recv_head);	

							pthread_cond_signal(&recv_cond);
							//pthread_cond_broadcast(&recv_cond);
							//pthread_mutex_unlock(&pThr->mutx);
							usleep(500);

						}
					}
				}
			}
		}else
		{
			pthread_mutex_lock(&pThr->mutx);
			pthread_cond_wait(&pThr->cond, &pThr->mutx);
			pthread_mutex_unlock(&pThr->mutx);
		}

	}// while(1)
	
	printf("thread is ended : thread_select\n");
	pthread_exit(NULL);
}


void *thread_recv(void *arg)
{
	thr_t *pThr;
	pThr = (thr_t *)arg;
	if(pThr == NULL)
	{
		printf("thread_recv is fail\n");
		pthread_exit(NULL);
	}
	else
		printf("START : %s\n", pThr->name);

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	int rc, str_len;
	char msg[BUF_SIZE];
	node_t *pNode = NULL;

	while(1)
	{
		//pthread_mutex_lock(&recv_mutx);
		rc = queue_isempty(&g_recv_head);
		//pthread_mutex_unlock(&recv_mutx);

		if(1 != rc)
		{		

			//pthread_mutex_lock(&recv_mutx);
			pNode = queue_get(&g_recv_head);	
			//pthread_mutex_unlock(&recv_mutx);

			if(pNode != NULL)
			{
				str_len = recv(pNode->fd, msg, BUF_SIZE, 0);
				if(1 < str_len)
				{
#if DEBUF_PRINT	
					printf("\t%s msg(%d) : %s\n", pThr->name, pNode->fd, msg);
#endif
					//pthread_mutex_lock(&recv_mutx);
					count_agent(&g_agent, pNode->fd);
					//pthread_mutex_unlock(&recv_mutx);
				}
				else if(0 == str_len)
				{
					// remove_agent
					//pthread_mutex_lock(&recv_mutx);
					remove_agent(&g_agent, pNode->fd);
					//pthread_mutex_unlock(&recv_mutx);
					if(g_agent.count == 0)
					{
						print_report(&g_report);
					}
		
				}else if(-1 == str_len)
				{
					printf("\tERROR(%d) in %s\n", pNode->fd, pThr->name);
				}else
				{

				}

				memset(msg, '\0', strlen(msg));
	
			}else
			{
				//printf("ERROR : pNode is NULL\n");
			}	

		}else
		{
			pthread_mutex_lock(&recv_mutx);
			pthread_cond_wait(&recv_cond, &recv_mutx);
			pthread_mutex_unlock(&recv_mutx);
			//printf("unlock : %s\n",pThr->name);
		}

	}// while(1)


	printf("thread is ended : thread_recv\n");
	pthread_exit(NULL);
}

void init_signal()
{
	struct sigaction sig_act;
	sigemptyset(&sig_act.sa_mask);			// signal.sa_mask 초기화
	sig_act.sa_handler = signal_handler;	// signal.sa_handler 연결
	sigfillset(&sig_act.sa_mask);			// 모든 signal 등록
	sigdelset(&sig_act.sa_mask, SIGINT);	// SIGINT 제외
	sig_act.sa_flags = 0;					// signal.sa_flag 초기화

	if(sigprocmask(SIG_BLOCK, &sig_act.sa_mask, 0) < 0)
	{
		perror("sigprocmaks");
		return ;
	}

	if(sigaction(SIGINT, &sig_act, 0) < 0)
	{
		perror("sigaction");
		return ;
	}
}
void signal_handler(int signum)
{
	printf("SIGNAL : SIGINT\n");
	printf("ctrl+c : exit\n");

	TAILQ_INIT(&g_recv_head);
	destroy_agentlist(&g_agent);

	signal(SIGINT, SIG_DFL);
}
