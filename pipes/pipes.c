/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:03:57 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/06 22:42:11 by pedperei         ###   ########.fr       */
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
	shell->args_execve = create_args_execve(shell);
	/* int j = -1;
		while (shell->args_execve[++j])
		{
			printf("%s\n", shell->args_execve[j]);
			
		} */
	while (pipe_index != -1)
	{
		handle_pipe(shell, &in_fd, pipe_index);
		i += pipe_index + 1;
		shell->index = i;
		shell->args_execve = create_args_execve(shell);
		pipe_index = find_pipe(shell, i);
		/* int j = -1;
		while (shell->args_execve[++j])
		{
			printf("%s\n", shell->args_execve[j]);
			
		}  */
	}
	i = -1;
	t_arg *temp;
	temp = get_arg_byindex(shell, shell->index);
	while(temp != NULL)
	{
		run_commands_aux(shell, in_fd, STDOUT_FILENO);
		while(temp)
		{
			if(strcmp(temp->name, "|") == 0 && !temp->quotes_perm)
			{
				temp = temp->next;
				break;
			}
			temp = temp->next;
		}
	}
	if (in_fd != 0)
		close(in_fd);
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
