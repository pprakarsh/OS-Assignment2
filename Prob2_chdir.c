#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

int main()
{

	char str_cwd[200];
	getcwd(str_cwd, 200);
	strcat(str_cwd, "/Hello");
	printf("%s", str_cwd);
}
