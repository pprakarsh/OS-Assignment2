#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

int main()
{
	char* s;
	printf("%s\n", getcwd(s, 100));
	char* s2 = strcat(s, "/Testing1");
	chdir(s2);
	printf("%s\n", getcwd(s2, 100));
}
