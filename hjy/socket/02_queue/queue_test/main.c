#include <stdio.h>
#include <stdlib.h>
//#include <sys/queue.h>
#include "queue.h"


typedef struct node{
	int client_fd;
	TAILQ_ENTRY(node) nodes;
}node_t;

int main(int arc, char *argv[])
{
	TAILQ_HEAD(head_s, node) head;
	// init()
	TAILQ_INIT(&head);
	
	// INSERT
	node_t *pNode = NULL;
	int i;
	for(i=0; i<10; i++)
	{
		pNode = (node_t*)malloc(sizeof(node_t));
		if(pNode == NULL)
		{
			printf("malloc failed\n");
		}
		pNode->client_fd = i;
		TAILQ_INSERT_TAIL(&head, pNode, nodes);
		printf("Insert : %d\n", pNode->client_fd);
		pNode = NULL;
	}
	
	// PRINT
	printf("Print\n");
	TAILQ_FOREACH(pNode, &head, nodes)
	{
		printf("%d\n", pNode->client_fd);
	}

	// free
	while(1)
	//while(!TAILQ_ENTRY(&head))
	{
		pNode = TAILQ_FIRST(&head);
		if(pNode == NULL)
			break;
		printf("free(%d)\n", pNode->client_fd);
		TAILQ_REMOVE(&head, pNode, nodes);
		free(pNode);
		pNode = NULL;

	}

	if(!TAILQ_EMPTY(&head))
			printf("tail queue is NOT empty\n");
	return 0;
}
