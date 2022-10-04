#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int error = errno;
	int argsRemain = argc;
	
	for (int i = 1; i < argc; i++)
	{
		int fd[2]; //Pipefd[0] - read end,  Pipefd[1] - write end
		if (pipe(fd) == -1) //Open pipe and catch error
		{
			return 1;
		}
		
		int pid = fork();
		if (pid == 0) //Child Process that runs argument
		{
			execlp(argv[i], argv[i], NULL);
			//printf("Argument %d - %s\n", i, argv[i]);
			exit(0);
		}
		else
		{
			waitpid(pid, NULL, 0);
		}

		close(fd[0]);
		close(fd[1]);
	}

	/*
	if (strcmp(argv[1],"ls") == 0)
	{
		execlp("ls", "ls", "-a", NULL);
	}
	*/
	return 0;
	
}