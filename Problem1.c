#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
struct queue
{
	int capacity;
	int size;
	int front;
	int rear;
	int* q;
};

struct shm_count
{
	int level;
	int nodes_at_level;
	int arr[10][1000];
	struct queue queue;
};

void make_queue(int cap, struct shm_count* ptr_node)
{
	ptr_node->queue.q = (int* )malloc(sizeof(int)*cap);	
	ptr_node->queue.capacity = cap;
	ptr_node->queue.front = 0;
	ptr_node->queue.rear = -1;
	ptr_node->queue.size = 0;
}
int isEmpty(struct shm_count* ptr_node)
{
	if(ptr_node->queue.size == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int isFull(struct shm_count* ptr_node)
{
	if(ptr_node->queue.size == ptr_node->queue.capacity-1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int pop(struct shm_count* ptr_node)
{
	int val = ptr_node->queue.q[ptr_node->queue.front];
	ptr_node->queue.front = (ptr_node->queue.front+1)%ptr_node->queue.capacity;
	ptr_node->queue.size -= 1;
	return val;
}
void push(struct shm_count* ptr_node, int val)
{
	ptr_node->queue.rear = (ptr_node->queue.rear+1)%ptr_node->queue.capacity;
	ptr_node->queue.q[ptr_node->queue.rear] = val;
	ptr_node->queue.size += 1;
}

int main(int argc, char* argv[])
{
	printf("\nHello, Chai Pilo");
	
	int v1 = atoi(argv[1]);
	int v2 = atoi(argv[2]);
	if(argc != 3)
	{
		printf("Total 3 arguments expected. Only 2 arguments given");
		exit(0);
	}
	
	key_t key = ftok("~/OS_Assignment2/shm_count.txt", 'k');
	int shm_id = shmget(key, sizeof(struct shm_count), 0666|IPC_CREAT);
	struct shm_count* ptr_node = shmat(shm_id, NULL, 0);

	make_queue(1000, ptr_node);
	printf("Hello %d\n",getpid());

	ptr_node->arr[0][0] = getpid();

	int i, r, c;
	r = 0;
	c = 0;
	
	for(i = 0; i < v1; i++)
	{
		pid_t p = fork();
		
		if(p < 0)
		{
			printf("Error! Forking not possible\n");
			exit(0);
		}	
		else if(p == 0)
		{
			c = i;
			r = 1;
			ptr_node->arr[r][c] = getpid();
			printf("\nptr_node->arr[r][c]: %d\n", ptr_node->arr[r][c]);
			kill(getpid(), SIGSTOP);
			printf("Hello I am child! row-%d :)%d\n", r, getpid());
			if(r == v2)
			{
				exit(0);
			}
			break;
		}
		else
		{
			continue;
		}
	}
	
	if(i == v1)
	{
		//Parent Process- checks if all children have been stopped
		for(int i = 0; i < v1; i++)
		{
			waitpid(ptr_node->arr[r+1][i], NULL, WUNTRACED);
		}
		for(int i = 0; i < v1; i++)
		{
			if(!isFull(ptr_node))
			{
				printf("\nPushing: %d", ptr_node->arr[r+1][i]);
				push(ptr_node, ptr_node->arr[r+1][i]);
			}
			else
			{
				printf("\nError! Queue is Full");
			}
		}

		int pid;
		if(!isEmpty(ptr_node))
		{
			pid = pop(ptr_node);
			printf("\npid print: %d, r-%d, c-%d\n", pid, r, c);
		}
		else
		{
			printf("\nError! Queue is empty");
		}
		kill(pid, SIGCONT);	

		//Parent Process- waits for all children to finish before exiting
		while(wait(NULL) != -1);
	}


	while(1)
	{
		printf("Hello, While loop\n");
		int j;
		for(j = 0; j < c+1; j++)
		{
			//printf("Hello %d\n",getpid());
			pid_t p = fork();
			
			if(p < 0)
			{
				printf("Error! Forking not possible\n");
				exit(0);
			}	
			else if(p == 0)
			{
				//printf("Hello. I am child! %d\n",getpid());
				c = c*(c+1)/2 + j;
				r = r+1;
				ptr_node->arr[r][c] = getpid();
				printf("\nptr_node->arr[r][c]: %d\n", ptr_node->arr[r][c]);
				kill(getpid(), SIGSTOP);
				printf("Hello I am child! row-%d :)%d\n", r, getpid());
				if(r == v2)
				{
					if(!isEmpty(ptr_node))
					{
						int pid = pop(ptr_node);
						kill(pid, SIGCONT);	
					}
					else
					{
						printf("\nError! Queue is empty");
					}
					exit(0);
				}
				break;
				
			}
			else
			{
				continue;
			}
			
		}
		if(j == c+1)
		{
			//Parent Process- checks if all children have been stopped
			for(int i = c*(c+1)/2; i < (c*(c+1)/2)+c+1; i++)
			{
				waitpid(ptr_node->arr[r+1][i], NULL, WUNTRACED);
			}
			for(int i = c*(c+1)/2; i < (c*(c+1)/2)+c+1; i++)
			{
				if(!isFull(ptr_node))
				{
					printf("\nPushing: %d", ptr_node->arr[r+1][i]);
					push(ptr_node, ptr_node->arr[r+1][i]);
				}
				else
				{
					printf("\nError! Queue is Full");
				}
			}

			int pid;
			if(!isEmpty(ptr_node))
			{
				pid = pop(ptr_node);
				printf("\npid print: %d, r-%d, c-%d\n", pid, r, c);
			}
			else
			{
				printf("\nError! Queue is empty");
			}
			kill(pid, SIGCONT);	
			
			//Parent: will wait for its children to terminate
			while(wait(NULL) != -1);
			exit(0);
		}
	}
	shmdt(ptr_node);
	shmctl(shm_id,IPC_RMID,NULL);
	for(int l = 0; l < v2; l++)
	{
		printf("\nLevel %d:  ", l);
		for(int j = 0; j < sizeof(ptr_node->arr[l])/sizeof(int); j++)
		{
			printf("%d ", ptr_node->arr[l][j]);
		}
	}
	exit(0);
}
