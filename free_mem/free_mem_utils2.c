/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:43:03 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/03 12:09:25 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_args(t_shell *shell, int len)
{
	if (shell->args_str)
		free_args_array(shell->args_str, len);
	if (shell->args)
		free_args_struct(shell->args);
}

void	free_env(t_shell *shell)
{
	if (shell->envs_str)
		free_env_array(shell->envs_str);
	if (shell->envs)
		free_env_struct(shell->envs);
}
