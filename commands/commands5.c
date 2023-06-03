/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 15:41:21 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/03 15:58:14 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo_command(char **av)
{
	int	new_line;
	int	i;

	new_line = 1;
	i = 1;
	if (av[i] && ft_strcmp(av[i], "-n") == 0)
	{
		new_line = 0;
		i++;
	}
	while (av[i])
	{
		write(STDOUT_FILENO, av[i], ft_strlen(av[i]));
		if (av[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (new_line)
		write(STDOUT_FILENO, "\n", 1);
}

void	get_env_name_and_value(char **env_name, char **env_value, char *new_var)
{
	char	*eq_pos;
	int		env_name_l;

	eq_pos = ft_strchr(new_var, '=');
	if (eq_pos)
		env_name_l = eq_pos - new_var;
	else
		env_name_l = ft_strlen(new_var);
	*env_name = malloc(env_name_l + 1);
	ft_strncpy(*env_name, new_var, env_name_l);
	(*env_name)[env_name_l] = '\0';
	*env_value = eq_pos + 1;
}



