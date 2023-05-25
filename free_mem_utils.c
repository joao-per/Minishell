/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 14:18:43 by pedperei          #+#    #+#             */
/*   Updated: 2023/05/25 20:16:49 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

void	free_string_array(char **str_arr)
{
	int	i;

	i = 0;
	while (str_arr[i])
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr[i]);
	//free(str_arr);
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

void	free_args_env(t_shell *shell)
{
	if (shell->args_str)
		free_string_array(shell->args_str);
	if (shell->args)
		free_args_struct(shell->args);
}
