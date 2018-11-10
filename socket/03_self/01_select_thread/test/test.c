
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>




void *thread_func(void *arg)
{
	int index = (intptr_t )arg;
	printf("thread(%ud) %d\n",pthread_self(), index);
	pthread_exit(NULL);
}


int main()
{
	pthread_t test[10];
	//int a[10];
	int i;
	int a;
	for(i=0; i<10; i++)
	{
		printf("main-thread : %d\n",i);
		//pthread_create(&test[i], NULL, thread_func, (void *)&i);
		pthread_create(&test[i], NULL, thread_func, (void *)(intptr_t)i);
		//pthread_create(&test[i], NULL, thread_func, (void *)&a);
	}

	for(i=0; i<10; i++)
	{
		pthread_join(test[i], NULL);
	}	

	return 0;
}


