/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:22:54 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/03 12:22:54 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_cd(t_shell *shell)
{
	if (ft_strcmp(shell->args_str[0], "cd") == 0)
	{
		cd_command(shell->args_str, shell->envs);
		return 0;
	}
	return 1;
}

int	handle_unset(t_shell *shell)
{
	int i;

	if (ft_strcmp(shell->args_str[0], "unset") == 0)
	{
		i = 1;
		while (shell->args_str[i])
		{
			unset_variable(shell->envs, shell->args_str[i]);
			i++;
		}
		return 0;
	}
	return 1;
}

int	handle_export(t_shell *shell)
{
	int i;

	if (ft_strcmp(shell->args_str[0], "export") == 0)
	{
		i = 1;
		while (shell->args_str[i])
		{
			export_variable(shell->envs, shell->args_str[i]);
			i++;
		}
		return 0;
	}
	return 1;
}
