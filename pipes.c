#include "Libft/libft.h"
#include "minishell.h"

int	find_pipe(char **av)
{
	int	i;

	i = 0;
	while (av[i] != NULL)
	{
		if (ft_strcmp(av[i], "|") == 0)
			return (i);
		i++;
	}
	return (-1);
}

/*
while the parent process is responsible for passing environment variables to the child and waiting for the child process to complete.
*/
void	file_descriptor_handler(int in, int out)
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

void	run_commands_aux(t_shell *shell, t_env **env_vars, int in_fd, int out_fd)
{
	pid_t	pid;
	int		status;
	int		built_in_command_executed;

	pid = fork();
	if (pid < 0)
	{
		printf("Fork failed.\n");
		exit(1);
	}
	else if (pid == 0)
	{
		file_descriptor_handler(in_fd, out_fd);
		handle_redirection(shell);
		//built_in_command_executed = check_commands(av, env_vars)
		// Return if a built-in command was executed
		built_in_command_executed = check_commands(shell, env_vars);
		if (!built_in_command_executed)
			exit(0);
		execute_external_command(shell, env_vars);
		perror("execve");
		exit(1);
	}
	else
	{
		//printf("waiting for %s\n", shell->args_str[0]);
		waitpid(pid, &status, 0);
		//printf("finished waiting for %s\n", shell->args_str[0]);
		check_commands2(shell, env_vars);
	}
}

void	execute_command(t_shell *shell, t_env **env_vars)
{
	int		pipe_index;
	int		fd[2];
	int		in_fd;
	char	**av;

	av = shell->args_str;
	(void)av;
	in_fd = 0;
	while ((pipe_index = find_pipe(shell->args_str)) != -1)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		free(shell->args_str[pipe_index]);
		shell->args_str[pipe_index] = NULL;
		run_commands_aux(shell, env_vars, in_fd, fd[1]);
		close(fd[1]);
		if (in_fd != 0)
			close(in_fd);
		in_fd = fd[0];
		shell->args_str += pipe_index + 1;
	}
	run_commands_aux(shell, env_vars, in_fd, STDOUT_FILENO);
	if (in_fd != 0)
		close(in_fd);
	shell->args_str = av;
}

void	execute_external_command(t_shell *shell, t_env **env_vars)
{
	char	*path_var;
	char	**path_dirs;
	char	*temp;
	char	*full_path;
	int		i;

	if (ft_strchr(shell->args_str[0], '/'))
	{
		execve(shell->args_str[0], shell->args_str, shell->envs_str);
		free_args(shell, shell->len_args);
		free(shell);
		perror("minishell");
		exit(1);
	}
	path_var = get_env_value("PATH", env_vars);
	path_dirs = ft_split(path_var, ':');
	free(path_var);
	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(temp, shell->args_str[0]);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			execve(full_path, shell->args_str, shell->envs_str);
			free(full_path);
			break ;
		}
		free(full_path);
		i++;
	}
	free_double_array(path_dirs);
	perror("minishell");
	exit(1);
}
