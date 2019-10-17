int pipe1[2];
int pipe2[2];

pipe(pipe1);
pipe(pipe2);

if(fork())
{
	//Main function
	while(wait(NULL) != -1);

	
		close(pipe1[0]);
		close(pipe1[1]);

		close(pipe2[1]);


		read(pipe2[0], &size, str);
}
else
{
	if(fork())
	{
		//ls execute
		close(pipe2[0]);
		close(pipe2[1]);

		close(pipe1[0]);

		dup2(pipe1[1], 1);
		close(pipe1[1]);

		execvl("ls", "ls", "-p", NULL);
		
	}
	else
	{
		close(pipe2[0]);
		close(pipe1[1]);

		dup2(pipe1[0], 0);
		close(pipe1[0]);

		dup2(pipe2[1], 1);
		close(pipe2[1]);
		
		execvl)	
		//grep execute
	}
}

