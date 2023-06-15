/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_aux1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:04:03 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/02 11:04:03 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_commands_aux(t_shell *shell, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		printf("Fork failed.\n");
		exit(1);
	}
	else if (pid == 0)
		handle_child_process(shell, in_fd, out_fd, pid);
	else
		check_commands2(shell, pid);
}

int	find_pipe(t_shell *shell, int pipe_index)
{
	int		pos;
	t_arg	*temp;

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

int	find_pipe_arg(t_shell *shell, int pipe_index)
{
	int		pos;
	t_arg	*temp;

	pos = 0;
	temp = get_arg_byindex(shell, pipe_index);
	while (temp)
	{
		if (ft_strcmp(temp->name, "|") == 0 && temp->quotes_perm == 0)
			return (pos);
		pos++;
		temp = temp->next;
	}
	return (-1);
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

void	try_execve_at_each_path(t_shell *shell, char **path_dirs)
{
	int		i;
	char	*full_path;

	i = 0;
	while (path_dirs[i])
	{
		full_path = construct_full_path(path_dirs[i], shell->args_execve[0]);
		if (access(full_path, X_OK) == 0)
		{
			execve(full_path, shell->args_execve, shell->envs_str);
			free(full_path);
			break ;
		}
		free(full_path);
		i++;
	}
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
