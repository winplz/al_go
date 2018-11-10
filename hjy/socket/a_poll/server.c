#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h> // using poll

#define PORT 4321
#define MAX 6		// server(1) + client(n)
#define BUF_SIZE 1024
#define DEBUG 0
int init_network();

// using select --> poll
int main()
{
	
	int server_sd, client_sd, max_idx;
	int client_len, str_len;
	struct sockaddr_in client_addr;
	int i, rc, nConnection = 0;
	char msg[BUF_SIZE];
	struct pollfd poll_fds[MAX];


	// init_network( socket --> bind --> listen )
	if( -1 == (server_sd = init_network()))
	{
		printf("error : init_network()\n");
		return -1;
	}
	
	client_len = sizeof(client_addr);

	// accept
	printf("sever_sd : %d\n", server_sd);
		
	// server_sd 로 0번째 index를 설정
	poll_fds[0].fd = server_sd;
	poll_fds[0].events = POLLIN;
	for(i=1; i<MAX; i++)
	{
		// pollfd 구조체 초기화 '-1' : 파일지시자 없음
		poll_fds[i].fd = -1;
		poll_fds[i].events = POLLIN | POLLERR;
		poll_fds[i].revents = 0;
	}
	max_idx = 0;

	// POLL
	while(1)
	{
		rc = poll(poll_fds, max_idx+1, 1000);
			
		// poll, return value check
		if(-1 == rc)
		{
			// error
			perror("poll");
			continue;
		}else if(0 == rc)
		{
			// timeout
			continue;
		}else
		{
			// poll_fds[0] 확인, accept() 
			if(poll_fds[0].revents & POLLIN)
			{
				client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
				if(-1 != client_sd)
				{
					for(i=1; i<=MAX; i++)
					{
						if(poll_fds[i].fd < 0)
						{
							poll_fds[i].fd = client_sd;
							if(max_idx < i)
								max_idx = i;
							printf("connection(%d)\n", client_sd);
							nConnection++;
							break;
						}
					}
				} // if(-1 == client_sd)
				else
				{
					perror("accept");
					continue;
				}
			} // if(poll_fds[0].revents & POLLIN

			// poll_fds[i] check 
			for(i=1; i<=max_idx; i++)
			{
				client_sd = poll_fds[i].fd;
				if(0 > client_sd)
				{
					// fd(-1) pass
#if DEBUG
					printf("client_sd is '-1'\n");
#endif
					continue;
				}
				else if(poll_fds[i].revents & POLLIN)
				{
					memset(msg, '\0', BUF_SIZE);
					str_len = recv(client_sd, msg, BUF_SIZE, 0);
					if(0 >= str_len)
					{
						printf("disconnection(%d)\n",client_sd);
						poll_fds[i].fd = -1;
						if(max_idx <= i)
							max_idx = i-1;
						close(client_sd);
						nConnection--;
					}else
					{
						printf("\tmsg(%d) : %s\n", poll_fds[i].fd, msg);
					}
					continue;
				}else if(poll_fds[i].revents & POLLIN)
				{
					printf("\tpoll_fds[%d] POLLERR\n");
					continue;
				}
			}
		}	// check : rc
	}	// while(1)


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
