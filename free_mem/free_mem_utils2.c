/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:43:03 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/03 19:23:53 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_args(t_shell *shell, int len)
{
	if (shell->args_str)
		free_args_array(shell->args_str, len);
	if (shell->args)
		free_args_struct(shell->args);
	if (shell->args_execve)
		free_args_array(shell->args_execve, count_strings(shell->args_execve));
}

void	free_env(t_shell *shell)
{
	if (shell->envs_str)
		free_env_array(shell->envs_str);
	if (shell->envs)
		free_env_struct(shell->envs);
}

void	cleanup_after_command(t_shell **shell)
{
	if (shell && *shell)
	{
		free_args(*shell, (*shell)->len_args);
		free(*shell);
		*shell = NULL;
	}
}
