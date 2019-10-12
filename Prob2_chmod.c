#include <sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>


int main()
{
	chmod("/home/prakarsh/OS_Assignment2/Folder_READONLY_BABA", S_IRUSR | S_IWUSR | S_IXUSR | S_IXUSR | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
}
