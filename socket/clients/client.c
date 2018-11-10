#include <stdio.h> //printf
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <pthread.h>

#define MAX 50
#define BUF_SIZE 4
#define MAX_BUF_SIZE 1024
#define PORT 4321
#define HOST "localhost"

#define SOCKET int

typedef struct {
	int fd;
	int cnt;
}node_t;

typedef struct {
	node_t list[MAX];
	int count;
	int total;
	struct timeval average;
}report_t;

report_t g_report;


void *connectionHandler(void *arg);
pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;

// 시간 측정을 위한
long long timeval_diff(struct timeval *diff,
         struct timeval *end,
         struct timeval *start);


int main(int argc, char *argv[])
{

	pthread_t connect_thr[MAX];
	int i;

	// init : g_report
	g_report.total;
	g_report.average.tv_sec = 0;
	g_report.average.tv_usec = 0;
	for(i=0; i<MAX; i++)
	{
		g_report.list[i].fd = -1;
		g_report.list[i].cnt = 0;
	}
	

	for(i=0; i<MAX; i++)
	{	
		usleep(100);
		pthread_create(&connect_thr[i], NULL, connectionHandler, NULL);
	}

	for(i=0; i<MAX; i++)
	{
		pthread_join(connect_thr[i], NULL);
	}

	 printf("----------------Report-----------------\n");
	 printf("| Total Connection : %2d\n", g_report.count);
	 printf("| Total log send count : %2d\n", g_report.total);
	 printf("| Average : %2ld sec %2ld usec\n",
			             (g_report.average.tv_sec/g_report.count),
			             (g_report.average.tv_usec/g_report.count));
	 printf("---------------------------------------\n");


	return 0;
}

void *connectionHandler(void *arg)
{
	int sd, i, rc, cnt = 0;
	struct timeval start, end, diff;
	struct sockaddr_in server;	
	char recvBuff[MAX_BUF_SIZE]={0,}, sendBuff[MAX_BUF_SIZE]={0,};
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

	// 시간측정 start
	gettimeofday(&start, NULL);

	printf("connection(%d)\n", sd);	
	pthread_mutex_lock(&mutx);
	int idx = g_report.count;
	g_report.list[idx].fd = sd;
	g_report.count++;
	pthread_mutex_unlock(&mutx);


	for(i=0; i<10; i++)	
	{
		//sleep(1);
		//usleep(100000); // 1 sec : 1000000
		//pthread_mutex_lock(&mutx);
		sprintf(sendBuff, "%d",sd, i);
		rc = send(sd, sendBuff, BUF_SIZE ,0);
		//rc = send(sd, sendBuff, strlen(sendBuff),0);
		//pthread_mutex_unlock(&mutx);
		if(rc < 0)
		{
			perror("send");
		}else
		{
			// count
			cnt++;
			pthread_mutex_lock(&mutx);
			g_report.list[idx].cnt++;
			pthread_mutex_unlock(&mutx);
		}
		//printf("%s\n", sendBuff);
		/*	
		rc = recv(sd, recvBuff, MAX_BUF_SIZE, 0);
		if(rc == 0)
		{
			perror("recv");
			break;
		}
		*/
		//printf("(%d)recv : %s\n", sd, recvBuff);
		memset(recvBuff, '\0', strlen(recvBuff));
		memset(sendBuff, '\0', strlen(sendBuff));
	}
	
	// 시간 측정 end
	gettimeofday(&end, NULL);
	timeval_diff(&diff, &end, &start);

	close(sd);
	printf("disconnection(%d) send_count(%d) time(%ld sec, %ld us)\n", sd, cnt,
			diff.tv_sec, diff.tv_usec);

	
	pthread_mutex_lock(&mutx);
	g_report.total += g_report.list[idx].cnt;
	g_report.average.tv_sec += diff.tv_sec;
	g_report.average.tv_usec += diff.tv_usec;
	pthread_mutex_unlock(&mutx);

	pthread_exit(NULL);
}



long long timeval_diff(struct timeval *diff,
		        struct timeval *end,
				struct timeval *start)
{
	    struct timeval temp_diff;
		if(diff == NULL)
			diff = &temp_diff;
			   
		diff->tv_sec = end->tv_sec - start->tv_sec;
		diff->tv_usec = end->tv_usec - start->tv_usec;
					 
		while(diff->tv_usec < 0)
		{
			diff->tv_usec += 1000000;
			diff->tv_sec -= 1;							
		}
		return (1000000LL * diff->tv_sec + diff->tv_usec);
}

