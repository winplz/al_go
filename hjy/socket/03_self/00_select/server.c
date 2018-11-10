#include "common.h"


#define CLIENT_CNT 100

int main()
{
	pthread_mutex_t mutx;
	pthread_mutex_init(&mutx, NULL);	

	int server_sd, client_sd;
	int client_len;
	struct sockaddr_in client_addr;

	fd_set temp_fds;
	int i, rc, str_len, nMax = 0;
	struct timeval timeout;
	char msg[BUF_SIZE];

	// init_network( socket --> bind --> listen )
	if( -1 == (server_sd = init_network()))
	{
		printf("error : init_network()\n");
		return -1;
	}

	// init : agent_list
	init_agentlist(&g_agent);
	init_agentlist(&g_report);
	
	client_len = sizeof(client_addr);

	// accept
	printf("sever_sd : %d\n", server_sd);
	FD_SET(server_sd, &g_agent.recv_fds);
	g_agent.max_fd = server_sd;	

	agent_node_t server_node;
	server_node.fd = server_sd;
	add_agent(&g_agent, server_node);

	while(1)
	{
		temp_fds = g_agent.recv_fds;

		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		
		//printf("select wait..\n");
		rc = select(g_agent.max_fd+1, &temp_fds, 0, 0, &timeout);
		
		if(-1 == rc)
		{
			perror("select");
			continue;
		}else if(0 == rc)
		{
			//printf("select : timeout\n");
		}else 
		{
			nMax = g_agent.count;
			for(i=0; i<=nMax; i++)
			{
				if(FD_ISSET(g_agent.list[i].fd, &temp_fds))
				{
					if(i == 0)
					{
						// accept
						pthread_mutex_lock(&mutx);
						client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
						
						agent_node_t new_agent;
						new_agent.fd = client_sd;
						add_agent(&g_agent, new_agent);
						pthread_mutex_unlock(&mutx);
					}
					else
					{
						rc = recv(g_agent.list[i].fd, msg, BUF_SIZE, 0);
						if(rc == 0)
						{
							pthread_mutex_lock(&mutx);
							remove_agent(&g_agent, g_agent.list[i].fd);
							pthread_mutex_unlock(&mutx);
							//printf("g_agent count = %d\n", g_agent.count);
							if(g_report.count >= CLIENT_CNT)
							{
								print_report(&g_report);
							}
						}else
						{
							//printf("recv_msg(%d) : %s\n",g_agent.list[i].fd, msg);	
							pthread_mutex_lock(&mutx);
							count_agent(&g_agent, g_agent.list[i].fd);
							pthread_mutex_unlock(&mutx);
						}
						memset(msg, '\0', BUF_SIZE);
					}
				}
			}
		}

	} // while(1)
	
	return 0;
}

