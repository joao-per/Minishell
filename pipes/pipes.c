/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:03:57 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/17 18:40:54 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

int	count_pipes(t_shell *shell)
{
	int		count;
	int		pos;
	t_arg	*temp;

	count = 0;
	pos = 0;
	temp = *shell->args;
	while (temp)
	{
		if (ft_strcmp(shell->args_str[pos], "|") == 0 && temp->quotes_perm == 0)
			count++;
		temp = temp->next;
		pos++;
	}
	return (count + 1);
}

void	run_last_command(t_shell *shell, int *in_fd)
{
	t_arg	*temp;

	temp = get_arg_byindex(shell, shell->index);
	while (temp != NULL)
	{
		run_commands_aux(shell, *in_fd, NULL);
		while (temp)
		{
			if (strcmp(temp->name, "|") == 0 && !temp->quotes_perm)
			{
				temp = temp->next;
				break ;
			}
			temp = temp->next;
		}
	}
}

void	pipe_loop(t_shell *shell, int *in_fd, int pipe_index)
{
	int		i;

	i = 0;
	while (pipe_index != -1)
	{
		handle_pipe(shell, in_fd, pipe_index);
		i += pipe_index + 1;
		shell->index = i;
		free_args_array(shell->args_execve, count_strings(shell->args_execve));
		shell->args_execve = create_args_execve(shell);
		pipe_index = find_pipe(shell, i);
		shell->current_cmd++;
	}
}

void	execute_command(t_shell *shell)
{
	int		pipe_index;
	int		*in_fd;
	char	**av;
	int		i;
	int		status;

	av = shell->args_str;
	in_fd = ft_calloc(1, sizeof(int));
	i = 0;
	pipe_index = find_pipe(shell, i);
	shell->cmds = count_pipes(shell);
	shell->current_cmd = 0;
	shell->args_execve = create_args_execve(shell);
	pipe_loop(shell, in_fd, pipe_index);
	run_last_command(shell, in_fd);
	while (shell->cmds > 0)
	{
		wait(&status);
		g_exit_status = WEXITSTATUS(status);
		shell->cmds--;
	}
	if (*in_fd != 0)
		close(*in_fd);
	shell->args_str = av;
}

void	execute_absolute_path(t_shell *shell)
{
	execve(shell->args_str[0], shell->args_execve, shell->envs_str);
	free_args(shell, shell->len_args);
	free(shell);
	perror("minishell");
	exit(1);
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
