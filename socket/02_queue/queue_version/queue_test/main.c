#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>


typedef struct node{
	int client_fd;
	TAILQ_ENTRY(node) nodes;
}node_t;

int main(int arc, char *argv[])
{
	TAILQ_HEAD(head_s, node) head, head2;
	// init()
	TAILQ_INIT(&head);
	TAILQ_INIT(&head2);

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
	printf("Print(1)\n");
	TAILQ_FOREACH(pNode, &head, nodes)
	{
		printf("%d\n", pNode->client_fd);
	}
	// 처음꺼 삭제
	if((pNode = TAILQ_FIRST(&head)) != NULL)
	{
		TAILQ_REMOVE(&head, pNode, nodes);
		free(pNode);
	}

	// PRINT
	printf("After delete, Print(1)\n");
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
		//printf("free(%d)\n", pNode->client_fd);
		TAILQ_REMOVE(&head, pNode, nodes);
		free(pNode);
		pNode = NULL;

	}

	if(!TAILQ_EMPTY(&head))
			printf("tail queue is NOT empty\n");
	return 0;
}
