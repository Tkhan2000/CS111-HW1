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
			perror("Error: unable to open pipe");
			exit(EXIT_FAILURE);
		}
		int pid = fork();
		if (pid == -1) //Create fork and catch error
		{
			perror("Error: unable to fork processes");
			exit(EXIT_FAILURE);
		}

		if (pid == 0) //Child Process that runs argument
		{
			if (i+1 < argc) //Check if there is an argument in front
			{
				dup2(fd[1], 1);// Redirect stdout to write end of pipe
				close(fd[0]);
			}

			execlp(argv[i], argv[i], NULL);
			exit(0);
		}
		else //Parent process that waits.
		{
			dup2(fd[0], 0); // Redirect stdin to read end of pipe
			close(fd[1]);
			waitpid(pid, NULL, 0);
		}

		close(fd[0]);
		close(fd[1]);
	}
	return 0;
	
}