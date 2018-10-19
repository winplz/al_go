#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 4321
#define MAX 1000
#define BUF_SIZE 1024

int init_network();

void *thread_recv(void *arg);

int main()
{

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
	
	while(1)
	{
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
				int client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
				printf("\t(%d)connection\n", client_sd);
				//pthread_create(&recv_thread, NULL, thread_recv,(void *)client_sd);
				pthread_create(&recv_thread, NULL, thread_recv,(void *)client_sd);
				//pthread_create(&recv_thread, NULL, thread_recv,(void *)(intptr_t)client_sd);
			}
		}
	}

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
	
	printf("Listenr on port : %d\n", PORT);
	printf("Waiting for connection\n");

	return server_sd;
}

void *thread_recv(void *arg)
{
	int sd = (intptr_t)arg;
	int rc;
	char msg[BUF_SIZE];
	
	printf("\t(%d)sock recv thread ready\n", sd);

	while((rc = recv(sd, msg, BUF_SIZE, 0)) > 0)
	{
		printf("\tmessage(%d) : %s\n", sd, msg);
		rc = send(sd, msg, strlen(msg), 0);
		if(rc < 0)
			perror("send");
		memset(msg, '\0', BUF_SIZE);	
	}
	
	printf("\tdisconnection(%d)\n", sd);
	close(sd);

	printf("\t(%d)sock recv thread ended\n", sd);
	pthread_exit(NULL);
}
