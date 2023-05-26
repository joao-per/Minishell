/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:18:43 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/26 19:46:12 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int count_strings(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
		i++;
	return (i);
}

void	free_env_array(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr[i]);
	free(str_arr);
}

void	free_args_array(char **str_arr, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr[i]);
	free(str_arr);
}

void	free_args_struct(t_arg **args)
{
	t_arg	*temp;
	t_arg	*temp2;

	temp = *args;
	while (temp)
	{
		free(temp->name);
		temp2 = temp;
		temp = temp->next;
		free(temp2);
	}
	free(args);
}

void	free_env_struct(t_env **envs)
{
	t_env	*temp;

	temp = *envs;
	while (*envs)
	{
		free((*envs)->env_name);
		free((*envs)->env_value);
		temp = *envs;
		*envs = (*envs)->next;
		free(temp);
	}
	free(envs);
}
