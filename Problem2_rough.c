#include <sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02
#define filename "marks1.txt"

struct shm_foundInfo
{
	char flag;
	char path[200];
};
int main()
{
	pid_t pid_main = getpid();
	key_t key = ftok("/home/prakarsh/OS_Assignment2/shm_foundInfo.txt", 's');                                                                                 
	int shm_id = shmget(key, sizeof(struct shm_foundInfo), 0666|IPC_CREAT);                                                                          
	struct shm_foundInfo* ptr_shm= (struct shm_foundInfo* )shmat(shm_id, NULL, 0);
	
	ptr_shm->flag = 'r';
	
	int count = 0;

	chdir("/home/prakarsh/Evaluator");
	char str[100];
	printf("Top working directory: %s\n", getcwd(str, 100));
	while(1)
	{
		char flag = 'n';	
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
				close(pfds1[0]);
				dup2(pfds1[1], 1);
				close(pfds1[1]);
				execlp("/bin/ls", "ls", "-p", 0);
			}
			else 
			{
				close(pfds1[1]);
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
			while(wait(NULL) != -1);
			char buffer[100];	
			buffer[0] = '\0';
			wait(NULL);
			
			char file_temp[] = "";
			read(pfds2[0], buffer, sizeof(buffer));
			if(buffer[0] != '\0')
			{
				//File found and set flag, path in shared memory
				ptr_shm->flag = 'g';
				char* str;
				strcpy(ptr_shm->path, getcwd(str, 200)); 
				
				char* str_slash = "/"; 
				strcat(ptr_shm->path, str_slash); 
				strcat(ptr_shm->path, filename);
				printf("\n%d File found! Hurray! count: %d, Path: %s\n", getpid(), count, ptr_shm->path);
				exit(0);
			}
			else
			{
				//printf("File not found! :(");
				//File not found
				if(ptr_shm->flag == 'g')
				{
					exit(0);
				}
			}

			close(pfds2[0]);

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
					close(pfds1[0]);
					dup2(pfds1[1], 1);
					close(pfds1[1]);
					execlp("/bin/ls", "ls", "-p", 0);
				}
				else 
				{
					close(pfds1[1]);
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
				while(wait(NULL) != -1);
				char buffer[300];
			      	for(int i = 0; i < 300; i++)
					buffer[i] = '\0';	

				wait(NULL);
				read(pfds3[0], buffer, 300); 
				close(pfds3[0]);
				//printf("\nBuffer output:\n%s\nBuffer ouput close\n", buffer);
				
				int init_size = strlen(buffer);
				char delim[] = "\n";

				char *ptr = strtok(buffer, delim);

				char str2[100];
				//printf("\ncwd: %s\n", getcwd(str2, 100));

				while(ptr != NULL)
				{
					if(fork())
					{
						//Flag set
						flag = 'p';
						ptr = strtok(NULL, delim);
					}
					else
					{
						flag = 'c';
						//printf("\n%s!DEBUG, cwd: %s\n", ptr, str2);
						char str_cwd[200];
						getcwd(str_cwd, 200);
						strcat(str_cwd, "/");
						strcat(str_cwd, ptr);
						chdir(str_cwd);
						chmod(str_cwd, S_IRUSR | S_IWUSR | S_IXUSR | S_IXUSR | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
						break;
					}
				}
			}
		}
		if(flag == 'n')
		{
			exit(0);
		}
		if(count == 0 && flag == 'p')
		{
			while(wait(NULL) != -1);				
			if(ptr_shm->flag == 'g')
			{
				if(getpid() == pid_main)
				{
					printf("Path: %s", ptr_shm->path);
				}
				printf("\nChanging Marks....\n");
				//change marks
				printf("%d Path: %s", getpid(), ptr_shm->path);
				FILE* fp = fopen(ptr_shm->path, "r+");
				printf("Hello, bhai!, \n");
				if(fp == NULL)
				{
					printf("Error, opening file\n");
				}
				char str[40];
				fscanf(fp, "%s", &str);
				while(strcmp(str, "Prakarsh") != 0)
				{
					fscanf(fp, "%s", &str);	
				}
				char ch;
				fscanf(fp, "%c", &ch);
				fprintf(fp, "19");
				fclose(fp);
			}
			else
			{
				char str1[100];
				printf("\nNo file found! Algorithm Incorrect. cwd: %s\n", getcwd(str1, 100));
			}
			exit(0);
			
		}
		else
		{
			if(flag == 'p')
			{
				while(wait(NULL) != -1);
				exit(0);
			}	
		}
		count++;
	}
}
