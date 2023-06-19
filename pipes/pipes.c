/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:03:57 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/19 20:29:09 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

void	run_last_command(t_shell *shell, int *in_fd)
{
	t_arg	*temp;

	temp = get_arg_byindex(shell, shell->index);
	while (temp != NULL)
	{
		run_commands_aux(shell, *in_fd, NULL);
		while (temp)
		{
			if (ft_strcmp(temp->name, "|") == 0 && !temp->quotes_perm)
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
	int		status;

	av = shell->args_str;
	in_fd = ft_calloc(1, sizeof(int));
	pipe_index = find_pipe(shell, 0);
	shell->cmds = count_pipes(shell);
	shell->current_cmd = 0;
	shell->args_execve = create_args_execve(shell);
	pipe_loop(shell, in_fd, pipe_index);
	run_last_command(shell, in_fd);
	while (shell->cmds > 0)
	{
		wait(&status);
		if (!is_builtin_command(shell) && g_check_exit[1] != 1)
			g_check_exit[0] = WEXITSTATUS(status);
		shell->cmds--;
	}
	if (*in_fd != 0)
		close(*in_fd);
	shell->args_str = av;
	free(in_fd);
}

char	*construct_full_path(char *path_dir, char *command)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(path_dir, "/");
	full_path = ft_strjoin(temp, command);
	free(temp);
	return (full_path);
}

void	handle_pipe(t_shell *shell, int *in_fd, int pipe_index)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	free(shell->args_str[pipe_index]);
	shell->args_str[pipe_index] = NULL;
	run_commands_aux(shell, *in_fd, fd);
	close(fd[1]);
	if (*in_fd != 0)
		close(*in_fd);
	*in_fd = fd[0];
	shell->args_str += pipe_index + 1;
}
