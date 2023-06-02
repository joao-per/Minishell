#include "../Libft/libft.h"
#include "../minishell.h"


int	find_pipe(t_shell *shell, int pipe_index)
{
	int pos;
	t_arg *temp;

	pos = 0;
	temp = get_arg_byindex(shell, pipe_index);
	while (temp)
	{
		if (ft_strcmp(shell->args_str[pos], "|") == 0 && temp->quotes_perm == 0)
			return (pos);
		pos++;
		temp = temp->next;
	}
	return (-1);
}

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

void	handle_child_process(t_shell *shell, int in_fd, int out_fd)
{
	int built_in_command_executed;

	file_descriptor_handler(in_fd, out_fd);
	handle_redirection(shell);
	built_in_command_executed = check_commands(shell);
	if (!built_in_command_executed)
		exit(0);
	execute_external_command(shell);
	perror("execve");
	exit(1);
}

void	run_commands_aux(t_shell *shell, int in_fd, int out_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		printf("Fork failed.\n");
		exit(1);
	}
	else if (pid == 0)
		handle_child_process(shell, in_fd, out_fd);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) 
			exit_status = WEXITSTATUS(status);
		check_commands2(shell);
	}
}

void	handle_pipe(t_shell *shell, int *in_fd, int pipe_index)
{
	int fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	free(shell->args_str[pipe_index]);
	shell->args_str[pipe_index] = NULL;
	run_commands_aux(shell, *in_fd, fd[1]);
	close(fd[1]);
	if (*in_fd != 0)
		close(*in_fd);
	*in_fd = fd[0];
	shell->args_str += pipe_index + 1;
}

void	execute_command(t_shell *shell)
{
	int		pipe_index;
	int		in_fd;
	char	**av;
	int		i;

	av = shell->args_str;
	in_fd = 0;
	i = 0;
	while ((pipe_index = find_pipe(shell, i)) != -1)
	{
		handle_pipe(shell, &in_fd, pipe_index);
		i += pipe_index + 1;
	}
	run_commands_aux(shell, in_fd, STDOUT_FILENO);
	if (in_fd != 0)
		close(in_fd);
	shell->args_str = av;
}

void	execute_absolute_path(t_shell *shell)
{
	execve(shell->args_str[0], shell->args_str, shell->envs_str);
	free_args(shell, shell->len_args);
	free(shell);
	perror("minishell");
	exit(1);
}

char	*construct_full_path(char *path_dir, char *command)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(path_dir, "/");
	full_path = ft_strjoin(temp, command);
	free(temp);

	return full_path;
}

void	try_execve_at_each_path(t_shell *shell, char **path_dirs)
{
	int		i;
	char	*full_path;

	i = 0;
	while (path_dirs[i])
	{
		full_path = construct_full_path(path_dirs[i], shell->args_str[0]);
		if (access(full_path, X_OK) == 0)
		{
			execve(full_path, shell->args_str, shell->envs_str);
			free(full_path);
			break ;
		}
		free(full_path);
		i++;
	}
}

void	execute_relative_path(t_shell *shell)
{
	char	*path_var;
	char	**path_dirs;

	path_var = get_env_value("PATH", shell->envs);
	path_dirs = ft_split(path_var, ':');
	free(path_var);

	try_execve_at_each_path(shell, path_dirs);

	free_double_array(path_dirs);
	perror("minishell");
	exit(1);
}


void	execute_external_command(t_shell *shell)
{
	if (ft_strchr(shell->args_str[0], '/'))
		execute_absolute_path(shell);
	else
		execute_relative_path(shell);
}



/* void	execute_external_command(t_shell *shell, t_env **env_vars)
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
} */
