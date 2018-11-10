#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 4321
#define MAX 100
#define BUF_SIZE 1024

int init_network();

int main()
{
	
	int server_sd, client_sd, max_sd;
	int client_len;
	struct sockaddr_in client_addr;

	fd_set recv_fds, send_fds, temp_fds;
	int i, rc;
	struct timeval timeout;

	int str_len;
	char msg[BUF_SIZE];

	// init_network( socket --> bind --> listen )
	if( -1 == (server_sd = init_network()))
	{
		printf("error : init_network()\n");
		return -1;
	}

	FD_ZERO(&recv_fds);
	FD_ZERO(&send_fds);
	
	client_len = sizeof(client_addr);

	// accept
	printf("sever_sd : %d\n", server_sd);
	FD_SET(server_sd, &recv_fds);
	max_sd = server_sd;	


	while(1)
	{
		temp_fds = recv_fds;	

		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		
		//printf("select wait..\n");
		rc = select(max_sd+1, &temp_fds, 0, 0, &timeout);
		
		if(-1 == rc)
		{
			perror("select");
			continue;
		}else if(0 == rc)
		{
			//printf("select : timeout\n");
		}else 
		{

			for(i=server_sd; i<=max_sd; i++)
			{
			
				if(FD_ISSET(i, &temp_fds))
				{
					if(i == server_sd)
					{
						// accept
						client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
						FD_SET(client_sd, &recv_fds);
						if(max_sd < client_sd)
							max_sd = client_sd;
						printf("connection(%d)\n", client_sd);
					}else
					{

						rc = recv(i, msg, BUF_SIZE, 0);
						if(rc == 0)
						{
							FD_CLR(i, &recv_fds);
							close(i);
							if(max_sd <= i)
								max_sd = i-1;
							printf("disconnection(%d)\n", i);
						}else
						{
							printf("\tmessage(%d): %s\n", i, msg);
						}
						memset(msg, '\0', BUF_SIZE);
					}
				}
			}
		}

	} // while(1)
	
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
 	int server_sd;
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
	
	printf("-----Listenr on port : %d\n", PORT);
	printf("-----Waiting for connection\n");

	return server_sd;
}
