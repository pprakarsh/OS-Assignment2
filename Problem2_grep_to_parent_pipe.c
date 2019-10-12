#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>

int main()
{
	int pfd[2];
	pipe(pfd);
	if(fork())
	{
	}
}
