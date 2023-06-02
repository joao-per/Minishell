/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_aux2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 11:04:26 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/02 11:04:26 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../Libft/libft.h"

void	handle_child_process(t_shell *shell, t_env **env_vars, int in_fd, int out_fd)
{
	int built_in_command_executed;

	file_descriptor_handler(in_fd, out_fd);
	handle_redirection(shell);
	built_in_command_executed = is_builtin_command(shell);
	check_commands(shell, env_vars);
	if (built_in_command_executed)
		exit(0);
	execute_external_command(shell, env_vars);
	perror("execve");
	exit(1);
}

void	handle_pipe(t_shell *shell, t_env **env_vars, int *in_fd, int pipe_index)
{
	int fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	free(shell->args_str[pipe_index]);
	shell->args_str[pipe_index] = NULL;
	run_commands_aux(shell, env_vars, *in_fd, fd[1]);
	close(fd[1]);
	if (*in_fd != 0)
		close(*in_fd);
	*in_fd = fd[0];
	shell->args_str += pipe_index + 1;
}
