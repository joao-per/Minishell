#include "minishell.h"
#include "Libft/libft.h"
 

/*
while the parent process is responsible for passing environment variables to the child and waiting for the child process to complete.
*/
void file_descriptor_handler(int in, int out)
{
	if (in != 0)
	{
		dup2(in, 0);
		close(in);
	}
	if (out != 1)
	{
		dup2(out, 1);
		close(out);
	}
}

void run_commands_aux(char **av, t_env **env_vars, int in_fd, int out_fd)
{
	pid_t pid;
	int status;
	int built_in_command_executed;

	pid = fork();
	if (pid < 0)
	{
		printf("Fork failed.\n");
		exit(1);
	}
	else if (pid == 0)
	{
		file_descriptor_handler(in_fd, out_fd);
		handle_redirection(av);
		//built_in_command_executed = check_commands(av, env_vars)
		// Return if a built-in command was executed
		built_in_command_executed = check_commands(av, env_vars);
		if (!built_in_command_executed)
			exit(0);
		execve(av[0], av, environ);
		perror("execve");
		exit(1);
	}
	else
	{
		/* int i=-1;
		while (av[++i])
		{
			printf("%s\n", av[i]);
		}
		printf("waiting for %s\n", av[0]); */
		waitpid(pid, &status, 0);
		//printf("finished waiting for %s\n", av[0]);
		check_commands2(av, env_vars);
	}
}

void execute_command(char **av, t_env **env_vars)
{
	int pipe_index;
	int fd[2];
	int in_fd;

	in_fd = 0;
	while ((pipe_index = find_pipe(av)) != -1)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		av[pipe_index] = NULL;
		run_commands_aux(av, env_vars, in_fd, fd[1]);
		close(fd[1]);
		if (in_fd != 0)
			close(in_fd);
		in_fd = fd[0];
		av += pipe_index + 1;
	}
	run_commands_aux(av, env_vars, in_fd, STDOUT_FILENO);
	if (in_fd != 0)
		close(in_fd);
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
		env_arr[i] = env_vars[i]->env_name;
		i++;
	}
	env_arr[env_count] = NULL;
	return (env_arr);
}