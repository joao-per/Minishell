/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 15:56:23 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/03 18:23:27 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin_command(t_shell *shell)
{
	if (ft_strcmp(shell->args_str[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "echo") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "cd") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "export") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "unset") == 0)
		return (1);
	if (ft_strcmp(shell->args_str[0], "env") == 0)
		return (1);
	return (0);
}

int	check_commands2(t_shell *shell, pid_t pid)
{
	if (pid != 0)
	{
		if (!handle_env(shell))
			return (0);
	}
	if (pid != 0)
	{
		if (!handle_export(shell))
			return (0);
	}
	if (pid != 0)
	{
		if (!handle_cd(shell))
			return (0);
	}
	return (1);
}

int	check_commands(t_shell *shell, pid_t pid)
{
	if (pid != 0)
	{
		if (!handle_export(shell))
			return (0);
	}
	if (!handle_pwd(shell))
		return (0);
	if (!handle_echo(shell))
		return (0);
	if (!handle_unset(shell))
		return (0);
	return (check_commands2(shell, pid));
}
