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

	if (ft_strcmp(shell->args_str[0], "pwd") == 0)
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			perror("minishell");
		else
		{
			write(STDOUT_FILENO, cwd, ft_strlen(cwd));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (0);
	}
	return (1);
}

int	handle_echo(t_shell *shell)
{
	if (ft_strcmp(shell->args_str[0], "echo") == 0)
	{
		echo_command(shell->args_str);
		return (0);
	}
	return (1);
}

int	handle_env(t_shell *shell)
{
	if (ft_strcmp(shell->args_str[0], "env") == 0)
	{
		print_env_vars(shell->envs);
		return (0);
	}
	return (1);
}

int	handle_export(t_shell *shell)
{
	int		i;

	if (ft_strcmp(shell->args_str[0], "export") == 0)
	{
		if (!shell->args_str[1])
		{
			print_export_vars(shell->envs);
			return (0);
		}
		i = 1;
		while (shell->args_str[i])
		{
			export_variable(shell->envs, shell->args_str[i]);
			i++;
		}
		return (0);
	}
	return (1);
}
