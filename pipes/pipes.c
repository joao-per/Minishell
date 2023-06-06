/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:03:57 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/06 12:23:16 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../minishell.h"

void	execute_command(t_shell *shell)
{
	int		pipe_index;
	int		in_fd;
	char	**av;
	int		i;

	av = shell->args_str;
	in_fd = 0;
	i = 0;
	pipe_index = find_pipe(shell, i);
	if (pipe_index != -1)
		create_args_pipe(shell);
	while (pipe_index != -1)
	{
		handle_pipe(shell, &in_fd, pipe_index);
		i += pipe_index + 1;
		pipe_index = find_pipe(shell, i);
	}
	i = -1;
	t_arg *temp;
	temp = *(shell->args);
	while(*(shell->args) != NULL)
	{
		run_commands_aux(shell, in_fd, STDOUT_FILENO);
		while(*(shell->args))
		{
			if(strcmp((*shell->args)->name, "|") == 0)
			{
				(*shell->args) = (*shell->args)->next;
				break;
			}
			(*shell->args) = (*shell->args)->next;
		}
	}
	*shell->args = temp;
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
