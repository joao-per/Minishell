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

/*
while the parent process is responsible for passing environment variables to the child and waiting for the child process to complete.
*/
void execute_command(char **av, t_env **env_vars)
{
	pid_t pid;
	int status; // Exit status of child process
	int pipe_fd[2];
	int is_builtin;
	int i = 0;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	status = 0;
	if (strcmp(av[0], "cd") == 0)
	{
		cd_command(av, env_vars);
		return;
	}
	pid = fork();
	if (pid < 0)
	{
		printf("Fork failed.\n");
		exit(1);
	}
	// Child process
	else if (pid == 0)
	{
		char buffer[MAX_LINE];
		int env_vars_count;
		env_vars_count = 0;
		close(pipe_fd[1]); // Close the write end of the pipe in the child process
		handle_redirection(av); // Apply redirection only to the child process

		// Add the following line to the child process before handling the commands
		dup2(pipe_fd[0], 3); // Duplicate the read end of the pipe to file descriptor 3
		close(pipe_fd[0]); // Close the original read end of the pipe

		// Deserialize the env_vars variable in the child process
		// Count the number of environment variables
		while (env_vars[env_vars_count] != NULL) {
			env_vars_count++;
		}
		// Allocate memory for the child_env_vars array
		t_env **child_env_vars = (t_env **)malloc((env_vars_count + 1) * sizeof(t_env *));
		int index = 0;
		// Read environment variables from the pipe (file descriptor 3) and store them in the child_env_vars array
		while (read(3, buffer, MAX_LINE) > 0 && buffer[0] != '\0') {
			child_env_vars[index] = (t_env *)malloc(sizeof(t_env));
			child_env_vars[index]->env_var = strdup(buffer);
			index++;
		}
		child_env_vars[index] = NULL;

		// Handle built-in commands using the child's environment variables.
		is_builtin = check_commands(av, env_vars);
		if (is_builtin == 0)
		{
			// Free child_env_vars memory
			while (child_env_vars[i] != NULL)
			{
				free(child_env_vars[i]->env_var);
				free(child_env_vars[i]);
				i++;
			}
			free(child_env_vars);
			exit(0);
		}
	}
	// Parent process / Wait for the child process to finish
	else
	{
		close(pipe_fd[0]); // Closes the read end of the pipe in the parent process, as the parent will only be writing to the pipe

		// Pass the env_vars variable to the child process through the pipe
		int env_vars_count = 0;
		// Write the env_vars variable to the pipe (fd 1) so the child process can read it
		while (env_vars[env_vars_count] != NULL)
		{
			write(pipe_fd[1], env_vars[env_vars_count]->env_var, strlen(env_vars[env_vars_count]->env_var) + 1);
			env_vars_count++;
		}
		write(pipe_fd[1], "", 1); // Write an empty string to indicate the end of the env_vars list
		close(pipe_fd[1]); // Close the write end of the pipe
		waitpid(pid, &status, 0); // Wait for the child process to finish
	}
}

void execute_external_command(char **av, t_env **env_vars)
{
	char	*path_var;
	char	**path_dirs;
	char	*cmd_path;
	char	*temp;
	char	*full_path;
	int		i;

	if (strchr(av[0], '/'))
	{
		execve(av[0], av, env_vars_to_char_arr(env_vars));
		perror("minishell");
		exit(1);
	}
	path_var = get_env_value("PATH", env_vars);
	if (!path_var)
	{
		perror("minishell: PATH not set");
		exit(1);
	}
	path_dirs = ft_split(path_var, ':');
	free(path_var);
	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
        full_path = ft_strjoin(temp, av[0]);
		free(temp);
		if (access(cmd_path, X_OK) == 0)
		{
			execve(cmd_path, av, env_vars_to_char_arr(env_vars));
			free(cmd_path);
			break;
		}
		free(cmd_path);
		i++;
	}

	free_double_array(path_dirs);
	perror("minishell");
	exit(1);
}

char **env_vars_to_char_arr(t_env **env_vars)
{
	int env_count;
	char **env_arr;
	int i;

	env_count = 0;
	while (env_vars[env_count] != NULL)
		env_count++;
	env_arr = (char **)malloc((env_count + 1) * sizeof(char *));
	i = 0;
	while (i < env_count)
	{
		env_arr[i] = env_vars[i]->env_var;
		i++;
	}
	env_arr[env_count] = NULL;
	return (env_arr);
}
