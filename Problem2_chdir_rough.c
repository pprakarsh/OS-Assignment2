#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

int main()
{

	char* str_cwd;
	getcwd(str_cwd, 100);
	strcat(str_cwd, "/Hello");
	printf("%s", str_cwd);
}
