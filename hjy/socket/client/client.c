#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#define MAX_MSG 1024
#define PORT 4321
#define HOST "localhost"

int main(int argc, char *argv[])
{
	int sd, rc, i;
	struct sockaddr_in server;
	char recvBuff[MAX_MSG], sendBuff[MAX_MSG];

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd == -1)
	{
		printf("Error : create\n");
		return 1;
	}
	printf("(1)Create socket\n");

	//server.sin_addr.s_addr = inet_addr("localhost");
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//server.sin_addr.s_addr = inet_addr(HOST);
	server.sin_family = AF_INET;
	server.sin_port = htons( PORT );
	

	rc = connect(sd, (struct sockaddr *)&server, sizeof(server));
	if(rc < 0)
	{
		perror("Error : connect");
		return 1;
	}
	printf("(2)Connect to server\n");

	while(1)
	{
		/*
		for(i=0; i<10; i++)
		{
			sprintf(sendBuff, "test(%d)", i);{
			rc = send(sd, sendBuff, strlen(sendBuff), 0);
			
		}
		*/
		printf("Send message : ");
		scanf("%s",&sendBuff);

			
		
		if(sendBuff[0] == 'q')
		{
			break;	
		}else
		{
			rc = send(sd, sendBuff, strlen(sendBuff), 0);
			if(rc < 0)
			{
				printf("Error : send\n");
				//break;
			}

			rc = recv(sd, recvBuff, MAX_MSG, 0);
			if(rc < 0)
			{
				printf("Error : recv\n");
				//break;
			}
			printf("Recv : %s\n", recvBuff);

		}
			
		memset(sendBuff, '\0', MAX_MSG);
		memset(recvBuff, '\0', MAX_MSG);
	}
	
	close(sd);

	return 0;
}
