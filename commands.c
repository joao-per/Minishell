/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:11:40 by joao-per          #+#    #+#             */
/*   Updated: 2023/04/12 23:17:46 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int	check_commands(char **av)
{
	char	cwd[MAX_LINE];

	if (strcmp(av[0], "cd") == 0)
	{
		if (av[1] == NULL)
			write(2, "minishell: expected argument to \"cd\"\n", 38);
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
