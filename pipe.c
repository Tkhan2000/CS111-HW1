#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

extern int errno;

int main(int argc, char *argv[])
{
	int status;
	for (int i = 1; i < argc; i++)
	{
		int fd[2]; //Pipefd[0] - read end,  Pipefd[1] - write end
		int exit[2]; // Use a seperate pipe to send child exit status to parent
		if (pipe(exit) == -1) //Open pipe and catch error
		{
			perror("Error: unable to open pipe");
			return errno;
		}
		if (pipe(fd) == -1) //Open pipe and catch error
		{
			perror("Error: unable to open pipe");
			return errno;
		}
		int pid = fork();
		if (pid == -1) //Create fork and catch error
		{
			perror("Error: unable to fork processes");
			return errno;
		}

		if (pid == 0) //Child Process that runs argument
		{
			close(exit[0]);
			if (i+1 < argc) //Check if there is an argument in front
			{
				dup2(fd[1], 1);// Redirect stdout to write end of pipe
				close(fd[0]); //Close read end of the pipe
			}

			if (execlp(argv[i], argv[i], NULL) == -1)
			{
				status = EINVAL;
				write(exit[1], &status, sizeof(status));
			}
		}
		else //Parent process that waits for child process.
		{
			int stat = 0; // stat stores status from pipe exit[]
			close(exit[1]);
			dup2(fd[0], 0); // Redirect stdin to read end of pipe
			close(fd[1]); // CLose the write end of the pipe
			read(exit[0], &stat, sizeof(stat));
			if (stat == EINVAL) //Check if the status from the child is EINVAL
			{
				return stat;
			}
			waitpid(pid, &status, 0);
		}

		close(fd[0]);
		close(fd[1]);
	}
	return 0;
	
}