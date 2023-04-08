#include "minishell.h"
#include "Libft/libft.h"

void	handle_pipe(char **av, int status, int i)
{
	int	pipe_fd[2];
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}

	pid_t pipe_pid = fork();
	if (pipe_pid < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (pipe_pid == 0)
	{
		close(pipe_fd[READ_END]);
		dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
		close(pipe_fd[WRITE_END]);
		execve(av[0], av, environ);
		perror("execve");
		exit(1);
	}
	else
	{
		close(pipe_fd[WRITE_END]);
		waitpid(pipe_pid, &status, 0);
		dup2(pipe_fd[READ_END], STDIN_FILENO);
		close(pipe_fd[READ_END]);
		execve(av[i + 1], av + i + 1, environ);
		perror("execve");
		exit(1);
	}
}

void handle_redirection(char **av)
{
	char	*input_file;
	char	*output_file;
	int		input_fd;
	int		output_fd;
	int		j;

	input_file = NULL;
	output_file = NULL;
	input_fd = 0;
	output_fd = 0;
	j = 0;
	while (av[j] != NULL)
	{
		if (strcmp(av[j], "<") == 0)
		// Input redirection
		{
			input_file = av[j + 1];
			input_fd = open(input_file, O_RDONLY);
			if (input_fd < 0)
			{
				perror("open");
				exit(1);
			}
			dup2(input_fd, STDIN_FILENO);
			close(input_fd);
			av[j] = NULL;
			break ;
		}
		else if (strcmp(av[j], ">") == 0)
		// Output redirection
		{
			output_file = av[j + 1];
			output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (output_fd < 0)
			{
				perror("open");
				exit(1);
			}
			dup2(output_fd, STDOUT_FILENO);
			close(output_fd);
			av[j] = NULL;
			break ;
		}
		j++;
	}
}

void	execute_command(char **av)
{
	pid_t	pid;
	int		pipe_flag;
	int		i;
	int		status;           // Exit status of child process

	status = 0;
	pipe_flag = 0;
	i = 0;
	pid = fork();
	if (pid < 0)
	{
		printf("Fork failed.\n");
		exit(1);
	}
	// Child process
	else if (pid == 0)
	{
		// Call handle_redirection function
		handle_redirection(av);
		// Check for piping and call handle_pipe function
		while (av[i] != NULL)
		{
			if (strcmp(av[i], "|") == 0)
			{
				pipe_flag = 1;
				av[i] = NULL;
				break ;
			}
			i++;
		}
		if (pipe_flag)
			handle_pipe(av + i + 1, status, i);
		else
		{
			execve(av[0], av, environ);
			perror("execve");
			exit(1);
		}
	}
	// Parent process / Wait for the child process to finish
	else
		waitpid(pid, &status, 0);
}
