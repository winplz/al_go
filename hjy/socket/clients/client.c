#include <stdio.h> //printf
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <pthread.h>

#define MAX 5
#define MAX_MSG 1024
#define PORT 4321
#define HOST "localhost"

#define SOCKET int

void *connectionHandler(void *arg);
SOCKET client_list[MAX];
SOCKET server_list[MAX];
int gIndex = 0;

pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{

	pthread_t connect_thr[MAX];
	int i;
	
	for(i=0; i<MAX; i++)
	{	
		usleep(1000);
		pthread_create(&connect_thr[i], NULL, connectionHandler, NULL);
	}

	for(i=0; i<MAX; i++)
	{
		pthread_join(connect_thr[i], NULL);
	}

	return 0;
}

void *connectionHandler(void *arg)
{
	
	int sd;
	struct sockaddr_in server;	
	int i=0, rc;
	char recvBuff[MAX_MSG]={0,}, sendBuff[MAX_MSG]={0,};
	memset(recvBuff, '\0', strlen(recvBuff));
	memset(sendBuff, '\0', strlen(sendBuff));

	sd = socket(AF_INET, SOCK_STREAM, 0);
        if(sd < 0)
        {
		perror("socket");
                return ;
        }

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        //server.sin_addr.s_addr = inet_addr(HOST);
        server.sin_family = AF_INET;
        server.sin_port = htons( PORT );


        rc = connect(sd, (struct sockaddr *)&server, sizeof(server));
        if(rc < 0)
        {
                perror("Error : connect");
                return ;
        }
	

	printf("connection(%lu) : %d\n,",pthread_self(), sd);

	//for(i=0; i<MAX; i++)
	for(i=0; i<10; i++)
	//{
		//while(1)
		{
		sleep(1);
		//usleep(100000); // 1 sec : 1000000
		//pthread_mutex_lock(&mutx);
		sprintf(sendBuff, "Send(%d) : data",sd);
		rc = send(sd, sendBuff, strlen(sendBuff),0);
		//pthread_mutex_unlock(&mutx);
		if(rc < 0)
		{
			perror("send");
		}
		//printf("%s\n", sendBuff);
		/*
		rc = recv(sd, recvBuff, MAX_MSG, 0);
		if(rc < 0)
		{
			perror("recv");
		}
		printf("(%d)recv : %s\n", sd, recvBuff);
		*/
		memset(recvBuff, '\0', strlen(recvBuff));
		memset(sendBuff, '\0', strlen(sendBuff));
		}
	//}

	close(sd);
	pthread_exit(NULL);
}
