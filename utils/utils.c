/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 12:16:03 by joao-per          #+#    #+#             */
/*   Updated: 2023/06/03 12:16:03 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_double_array(char **doubles)
{
	int	i;

	i = 0;
	if (!doubles)
		return ;
	while (doubles[i] != NULL)
	{
		free(doubles[i]);
		i++;
	}
	free(doubles);
}

int	check_exit(t_shell *shell)
{
	t_arg			*temp;
	unsigned char	c;

	temp = (*shell->args);
	if (!(shell->args))
		return (2);
	if (temp == NULL)
		return (2);
	else if (ft_strcmp(temp->name, "exit") == 0 && count_pipes(shell) == 1)
	{
		if (temp->next)
		{
			c = ft_atoi(temp->next->name);
			g_check_exit[0] = (int)c;
			exit((int)c);
		}
		printf("exit\n");
		return (-1);
	}
	else if (ft_strcmp(temp->name, "exit") == 0 && count_pipes(shell) > 1)
		g_check_exit[0] = 0;
	return (1);
}
