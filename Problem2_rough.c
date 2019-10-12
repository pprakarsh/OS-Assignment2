#include<fcntl.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02
int main()
{
	int pfds2[2];
	if(pipe(pfds2) == -1)
	{
		printf("Cannot pipe, error!");
	}

	if(fork() == 0)
	{
		int pfds1[2];
		pipe(pfds1);
		close(pfds2[0]);
		if(fork() > 0)
		{
			close(pfds2[1]);
			printf("Hello, I am 2\n");
			close(pfds1[0]);
			dup2(pfds1[1], 1);
			close(pfds1[1]);
			execlp("/bin/ls", "ls", "-p", 0);
		}
		else 
		{
			close(pfds1[1]);
			printf("Hello, I am 3\n");
			if(dup2(pfds1[0], 0) == -1)
			{
				printf("grep input redirect error!\n");
			}
			close(pfds1[0]);
			int st;
			if(st = dup2(pfds2[1], 1) == -1)
			{
				printf("grep ouput redirect error!\n");
			}
			close(pfds2[0]);

			//char* input_str = "How are you?";
			//write(pfds2[1], input_str, strlen(input_str));
			execlp("/bin/grep", "grep", "-v", "/", 0);
		}
	}
	else 
	{
		close(pfds2[1]);
		while(wait(NULL) != -1);
		//We will read from output of (ls -p | grep -v "/") and (ls -p | grep "/")
		//and we will search for required file and recurse through directories
		printf("Hello, I am Grand Parent\n");
		while(wait(NULL) != -1);
		char buffer[200];	
		char temp[1];
		wait(NULL);
		printf("HI am Joker\n");
		temp[0] = ' ';
		read(pfds2[0], buffer, 200); 
		close(pfds2[0]);
		printf("Buffer: %s", buffer);
		printf("Hello, I am 1\n");

	}
}
