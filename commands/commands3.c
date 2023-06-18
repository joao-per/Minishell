/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2023/06/03 12:27:11 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/03 12:27:11 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pwd(t_shell *shell)
{
	char	cwd[MAX_LINE];

	if (ft_strcmp(shell->args_execve[0], "pwd") == 0)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			perror("minishell");
		else
		{
			write(STDOUT_FILENO, cwd, ft_strlen(cwd));
			write(STDOUT_FILENO, "\n", 1);
			g_check_exit[0] = 0;
		}
		return (0);
	}
	return (1);
}

int	handle_echo(t_shell *shell)
{
	if (ft_strcmp(shell->args_execve[0], "echo") == 0)
	{
		echo_command(shell->args_execve);
		g_check_exit[0] = 0;
		return (0);
	}
	return (1);
}

int	handle_env(t_shell *shell)
{
	if (ft_strcmp(shell->args_execve[0], "env") == 0
		&& (!shell->args_execve[1] || ft_strcmp(shell->args_execve[1],
				"env") == 0))
	{
		print_env_vars(shell->envs);
		g_check_exit[0] = 0;
		return (0);
	}
	else if (ft_strcmp(shell->args_execve[0], "env") == 0
		&& shell->args_execve[1])
	{
		printf("env: ‘%s’: No such file or directory\n",
			shell->args_execve[1]);
		g_check_exit[0] = 127;
		return (0);
	}
	return (1);
}

int	handle_export(t_shell *shell)
{
	int		i;
	int		change;

	change = 0;
	if (ft_strcmp(shell->args_execve[0], "export") == 0)
	{
		if (!shell->args_execve[1])
		{
			print_export_vars(shell->envs);
			g_check_exit[0] = 0;
			return (0);
		}
		i = 0;
		while (shell->args_execve[++i])
		{
			if (export_variable(shell->envs, shell->args_execve[i]))
				change = 1;
		}
		if (change)
			g_check_exit[0] = 1;
		else
			g_check_exit[0] = 0;
		return (0);
	}
	return (1);
}
