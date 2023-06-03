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

void cd_command(char **av, t_env **env_vars)
{
	char	cwd[MAX_LINE];
	char	*oldwd;
	t_env 	*temp;

    if (av[1] == NULL)
		chdir(getenv("HOME"));
	else
	{
		if (chdir(av[1]) != 0)
		{
			perror("minishell");
			return;
		}
	}
	getcwd(cwd, sizeof(cwd));
	temp = search_env_name(env_vars, "PWD");
	oldwd = ft_strdup(temp->env_value);
	free(temp->env_value);
	temp->env_value = ft_strdup(cwd);
	temp = search_env_name(env_vars, "OLDPWD");
	free(temp->env_value);
	temp->env_value = ft_strdup(oldwd);
	free(oldwd);
}

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
