/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:11:40 by joao-per          #+#    #+#             */
/*   Updated: 2023/04/16 10:13:20 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int	check_commands(char **av, t_env **env_vars)
{
	char	cwd[MAX_LINE];

	if (strcmp(av[0], "cd") == 0)
	{
		if (av[1] == NULL)
			chdir(getenv("HOME"));
		else
		{
			if (chdir(av[1]) != 0)
				perror("minishell");
		}
		return (0);
	}
	if (strcmp(av[0], "pwd") == 0)
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
	if (strcmp(av[0], "export") == 0)
	{
		export_variable(env_vars, av[1]);
		return (0);
	}
	return (check_commands2(av));
}

int	check_commands2(char **av)
{
	struct dirent	*entry;
	DIR				*dir;
	char			*dir_path;

	if (strcmp(av[0], "ls") == 0)
	{
		if (av[1] == NULL)
			dir_path = ".";
		else
			dir_path = av[1];
		if ((dir = opendir(dir_path)) == NULL)
			perror("minishell");
		else
		{
			while ((entry = readdir(dir)) != NULL)
			{
				write(1, entry->d_name, ft_strlen(entry->d_name));
				write(STDOUT_FILENO, "\n", 1);
			}
			closedir(dir);
		}
		return (0);
	}
	return (1);
}

void	export_variable(t_env **env_vars, const char *new_var)
{
	t_env	*new_env_var;
	t_env	*curr;

	// If no argument is provided, display all environment variables
	if (!new_var)
	{
		print_env_vars(env_vars);
		return ;
	}
	new_env_var = (t_env *)malloc(sizeof(t_env));
	if (!new_env_var)
	{
		perror("minishell: export: malloc");
		return ;
	}
	new_env_var->env_var = ft_strdup(new_var);
	new_env_var->next = NULL;
	if (*env_vars == NULL)
	{
		*env_vars = new_env_var;
		return ;
	}
	curr = *env_vars;
	while (curr->next)
		curr = curr->next;
	curr->next = new_env_var;
}

