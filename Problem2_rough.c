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
#define filename "queue.c"

int main()
{
	int pfds2[2];

	pipe(pfds2);

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
			execlp("/bin/grep", "grep", "-w", filename, 0);
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
		char buffer[100];	
		buffer[0] = '\0';
		wait(NULL);
		
		char file_temp[] = "";
		read(pfds2[0], buffer, sizeof(buffer));
		if(buffer[0] != '\0')
		{
			printf("File found! Hurray!\n");
			//File found and set flag, path in shared memory

		}
		else
		{
			printf("File not found! :(");
			//File not found
		}
/*		while(read(pfds2[0], buffer, 1) > 0) 
 
		{
			if(buffer[0] == '\n' )
			{
//				file_temp[0] = '\0';
			}
			else
			{
				//char temp_buffer[2];
				//temp_buffer[0] = buffer[0];
				//temp_buffer[1] = '\0';
				//strcat(file_temp, temp_buffer);
				if(strcmp(file_temp, "shm_count.txt") == 0)
				{
					printf("Found file! %s", file_temp);
					//Set the flag

					break;
				}
			}
		}*/
		close(pfds2[0]);
		printf("Hello, I am 1\n");

		int pfds3[2];
		pipe(pfds3);
		if(fork() == 0)
		{
			int pfds1[2];
			pipe(pfds1);
			close(pfds3[0]);
			if(fork() > 0)
			{
				close(pfds3[1]);
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
				if(st = dup2(pfds3[1], 1) == -1)
				{
					printf("grep ouput redirect error!\n");
				}
				close(pfds3[0]);

				//char* input_str = "How are you?";
				//write(pfds2[1], input_str, strlen(input_str));
				execlp("/bin/grep", "grep", "/", 0);
			}
		}
		else
		{
			close(pfds3[1]);
			while(wait(NULL) != -1);
			//We will read from output of (ls -p | grep -v "/") and (ls -p | grep "/")
			//and we will search for required file and recurse through directories
			printf("Hello, I am Grand Parent\n");
			while(wait(NULL) != -1);
			char buffer[300];	
			wait(NULL);
			read(pfds3[0], buffer, 300); 
			close(pfds3[0]);
			printf("Buffer: %s", buffer);
			printf("Hello, I am 1\n");
		}
	}
}
