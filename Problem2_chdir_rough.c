#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

int main()
{
	printf("Hello");
	chdir("./Testing1");
	printf("Hello");
	char str_cwd[100];
	getcwd(str_cwd, 100);
	strcat(str_cwd, "/Hello");
	printf("%s", str_cwd);
}
