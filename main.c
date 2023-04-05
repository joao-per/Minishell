#include "minishell.h"
#include "Libft/libft.h"

#define MAX_LINE 80 // Maximum length of user input
#define MAX_ARGS MAX_LINE/2 + 1 // Maximum number of arguments for a command
#define READ_END 0 // File descriptor for read end of a pipe
#define WRITE_END 1 // File descriptor for write end of a pipe

extern char **environ; // Add this after the includes

void parse_input(char *input, char **args, const char *delimiter)
{
	int i;
	int j;
	int start;

	i = 0;
	j = 0;
	start = -1;
	while (input[j] != '\0')
	{
		if (start == -1 && input[j] != *delimiter)
			start = j;
		else if (start != -1 && (input[j] == *delimiter || input[j + 1] == '\0'))
		{
			if (input[j + 1] == '\0')
				j++;
			args[i++] = ft_strndup(input + start, j - start);
			start = -1;
		}
		j++;
	}
	args[i] = NULL; // Add NULL terminator to the end of args array
}

int main(void)
{
	char input[MAX_LINE]; // Buffer to store user input
	char *args[MAX_ARGS]; // Array to store parsed user input
	int should_run = 1; // Flag to indicate when to exit the loop
	pid_t pid; // Process ID of child process
	int status; // Exit status of child process
	int i = 0;

	while (should_run) {
		write(STDOUT_FILENO, "\033[0;93m Minishell>$ \033[0;39m", 28);

		// Read in user input using read
		ssize_t n = read(STDIN_FILENO, input, MAX_LINE);
		if (n == -1)
		{ // Error occurred
			perror("read");
			continue ;
		}
		else if (n == 0)
		{ // End of file (e.g. user pressed Ctrl-D)
			should_run = 0;
			break ;
		}
		input[n - 1] = '\0'; // Replace newline character with null terminator

		// Parse the input into separate arguments
		parse_input(input, args, " ");

		// Check for built-in commands
		if (args[0] == NULL)
			continue ;
		else if (strcmp(args[0], "exit") == 0)
		{
			should_run = 0;
			break;
		}
		// We can add code for commands HERE!!!!!

		// Fork a child process to execute the command
		pid = fork();

		if (pid < 0)
		{ // Error handling
			printf("Fork failed.\n");
			exit(1);
		}
		else if (pid == 0)
		// Child process
		{
			// Check for input/output redirection
			int input_fd = 0, output_fd = 0;
			char *input_file = NULL, *output_file = NULL;
			int j = 0;
			while (args[j] != NULL)
			{
				if (strcmp(args[j], "<") == 0)
				// Input redirection
				{
					input_file = args[j + 1];
					input_fd = open(input_file, O_RDONLY);
					if (input_fd < 0) {
						perror("open");
						exit(1);
					}
					dup2(input_fd, STDIN_FILENO);
					close(input_fd);
					args[j] = NULL;
					break;
				}
				else if (strcmp(args[j], ">") == 0)
				// Output redirection
				{
					output_file = args[j + 1];
					output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (output_fd < 0)
					{
						perror("open");
						exit(1);
					}
					dup2(output_fd, STDOUT_FILENO);
					close(output_fd);
					args[j] = NULL;
					break;
				}
				j++;
			}
			// Check for piping
			int pipe_fd[2];
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				exit(1);
			}
			pid_t pipe_pid;
			if ((pipe_pid = fork()) < 0)
			// Error handling
			{
				perror("fork");
				exit(1);
			}
			else if (pipe_pid == 0)
			{ // Child process for pipe
				close(pipe_fd[READ_END]);
				dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
				close(pipe_fd[WRITE_END]);
				execve(args[0], args, environ);
				perror("execve");
				exit(1);
			}
			else { // Parent process
				close(pipe_fd[WRITE_END]);
				waitpid(pipe_pid, &status, 0); // Wait for the child process to finish
				dup2(pipe_fd[READ_END], STDIN_FILENO);
				close(pipe_fd[READ_END]);
				execve(args[i + 1], args + i + 1, environ); 
				perror("execve");
				exit(1);
			}
			// End of code for piping

			execve(args[0], args, environ);
			perror("execve");
			exit(1);
		}
		else // Parent process
			waitpid(pid, &status, 0); // Wait for the child process to finish
	}
	return 0;
}
				   
