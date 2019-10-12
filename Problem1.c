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
	int q[1000];
};

struct shm_count
{
	int level;
	int nodes_at_level;
	int arr[10][1000];
	struct queue queue;
	int flag;
};

void make_queue(int cap, struct shm_count* ptr_node)
{
	ptr_node->queue.capacity = cap;
	printf("Hello\n");
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
	int v1 = atoi(argv[1]);
	int v2 = atoi(argv[2]);
	if(argc != 3)
	{
		printf("Total 3 arguments expected. Only 2 arguments given");
		exit(0);
	}
	
	key_t key = ftok("~/OS_Assignment2/shm_count_new.txt", 's');
	int shm_id = shmget(key, sizeof(struct shm_count), 0666|IPC_CREAT);
	struct shm_count* ptr_node = shmat(shm_id, NULL, 0);

	make_queue(1000, ptr_node);

	ptr_node->arr[0][0] = getpid();

	int i, r, c;
	r = 0;
	c = 0;
	
	ptr_node->flag = 0;	
	
	// sleep(5);
	int count = 0;
	while(1)
	{
		int j;
		for(j = 0; (j < c+1) || (count == 0 && j < v1); j++)
		{
			pid_t p = fork();
			
			if(p < 0)
			{
				printf("Error! Forking not possible\n");
				exit(0);
			}	
			else if(p == 0)
			{
				c = c*(c+1)/2 + j;
				r = r+1;
				ptr_node->arr[r][c] = getpid();
				kill(getpid(), SIGSTOP);
				printf("Hello I am child! row-%d :)%d\n", r, getpid());
				sleep(2);
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
					sleep(33);
					exit(0);
				}
				break;
					
			}
			else
			{
				waitpid(p, NULL, WUNTRACED);

				if(!isFull(ptr_node))
				{
					push(ptr_node, p);
				}
				else
				{
					printf("\nError! Queue is Full");
				}

				continue;
			}
			
		}

		if(j == c+1 || (count == 0 && j == v1))
		{
			int pid;
			if(!isEmpty(ptr_node))
			{
				pid = pop(ptr_node);
			}
			else
			{
				printf("\nError! Queue is empty");
			}
			kill(pid, SIGCONT);	
			
			//Parent: will wait for its children to terminate
			while(wait(NULL) != -1);
			if(count != 0)
			{
				exit(0);
			}
			else
			{
				break;
			}
		}
		count++;
	}

	int n_nodes[v2+1];
	int prev = v1;
	n_nodes[0] = 1;
	for(int i = 1; i <= v2; i++)
	{
		n_nodes[i] = prev;
		prev = prev*(prev+1)/2;
	}
	for(int l = 0; l <= v2; l++)
	{
		printf("\nLevel %d:  ", l);
		for(int j = 0; j < n_nodes[l]; j++)
		{
			printf("%d ", ptr_node->arr[l][j]);
		}
	}
	printf("\n");

	shmdt(ptr_node);
	shmctl(shm_id,IPC_RMID,NULL);
	exit(0);
}
