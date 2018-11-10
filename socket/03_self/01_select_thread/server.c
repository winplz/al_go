#include "common.h"


void *thread_recv(void *arg);

pthread_mutex_t mutx;

int main()
{
	pthread_mutex_init(&mutx, NULL);

	int server_sd, max_sd;
	int client_len;
	struct sockaddr_in client_addr;
	fd_set recv_fds;
	int i, rc;

	pthread_t recv_thread;

	// init_network( socket --> bind --> listen )
	if( -1 == (server_sd = init_network()))
	{
		printf("error : init_network()\n");
		return -1;
	}
	printf("sever_sd : %d\n",server_sd);

	FD_ZERO(&recv_fds);
	FD_SET(server_sd, &recv_fds);
	max_sd = server_sd;
				
	client_len = sizeof(client_addr);
	
	init_agentlist(&g_agent);
	init_agentlist(&g_report);

	while(1)
	{
		if(g_report.count >= 50)
			print_report(&g_report);
		// accept 만 체크하기 때문에 timeout 설정 필요 없음
		rc = select(server_sd+1, &recv_fds, 0, 0, NULL);

		if(-1 == rc)
		{
			perror("select");
		}else if(0 == rc)
		{
			printf("select : timeout\n");
		}else
		{

			if(FD_ISSET(server_sd, &recv_fds))
			{
				pthread_mutex_lock(&mutx);	
				int client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
				//printf("\t(%d)connection\n", client_sd);
				agent_node_t new_agent;
				new_agent.fd = client_sd;
				add_agent(&g_agent, new_agent);
				pthread_mutex_unlock(&mutx);	

				pthread_create(&recv_thread, NULL, thread_recv,(void *)(intptr_t)client_sd);
			}
		}
	}

	return 0;
}


void *thread_recv(void *arg)
{
	int sd = (intptr_t)arg;
	int rc;
	char msg[BUF_SIZE];	
	memset(msg, '\0', BUF_SIZE);	

	//printf("\t(%d)sock recv thread ready\n", sd);

	while((rc = recv(sd, msg, BUF_SIZE, 0)) > 0)
	{
		//printf("\tmessage(%d) : %s\n", sd, msg);
		pthread_mutex_lock(&mutx);	
		count_agent(&g_agent, sd);
		pthread_mutex_unlock(&mutx);	
		memset(msg, '\0', BUF_SIZE);	
	}
	
	//printf("\tdisconnection(%d)\n", sd);
				
	pthread_mutex_lock(&mutx);	
	remove_agent(&g_agent, sd);
	pthread_mutex_unlock(&mutx);	
	
	//printf("\t(%d)sock recv thread ended\n", sd);
	pthread_exit(NULL);
}
