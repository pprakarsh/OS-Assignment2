#include<stdio.h>
struct queue
{
	int capacity;
	int size;
	int front;
	int rear;
	int* q;
};


void make_queue(int cap)
{
	ptr_node->q = (int* )malloc(sizeof(int)*cap);	
	ptr_node->capacity = cap;
	ptr_node->front = 0;
	ptr_node->rear = -1;
	ptr_node->size = 0;
}
int isEmpty()
{
	if(ptr_node->size == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int isFull()
{
	if(ptr_node->size == ptr_node->capacity-1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int pop()
{
	ptr_node->front = (ptr_node->front+1)%ptr_node->capcaity;
	ptr_node->size -= 1;
	return ptr_node->q[ptr_node->front];
}
void push()
{
	ptr_node->rear = (ptr_node->rear+1)%ptr_node->capacity;
	ptr_node->size += 1;
}

int main()
{
struct queue* ptr_node = (struct queue* )malloc(sizeof(struct queue));
	make_queue(5);	

	push(4);
	push(4);
	push(3);
	push(8);

	while(!isEmpty())
	{
		printf("\nFirst Item: %d", pop());	
	}
}
