/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:17:23 by pedperei          #+#    #+#             */
/*   Updated: 2023/06/17 19:58:20 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env_vars(t_env **env_arr)
{
	t_env	*current;

	current = *env_arr;
	while (current)
	{
		if (current->env_value)
			printf("%s=%s\n", current->env_name, current->env_value);
		current = current->next;
	}
}

void	rank_export(t_env **env_arr)
{
	t_env	*temp;
	t_env	*comp;
	int		rank;

	temp = (*env_arr);
	comp = (*env_arr);
	while (temp != NULL)
	{
		rank = 1;
		comp = (*env_arr);
		while (comp != NULL)
		{
			if (ft_strcmp(comp->env_name, temp->env_name) < 0)
				rank++;
			comp = comp->next;
		}
		temp->rank = rank;
		temp = temp->next;
	}
}

void	print_export_aux(t_env *env_var)
{
	if (env_var->env_value)
		printf("declare -x %s=\"%s\"\n", env_var->env_name, env_var->env_value);
	else
		printf("declare -x %s\n", env_var->env_name);
}

void	print_export_vars(t_env **env_arr)
{
	t_env	*current;
	t_env	*comp;
	int		rank;

	rank = 1;
	current = *env_arr;
	comp = *env_arr;
	rank_export(env_arr);
	while (current)
	{
		comp = *env_arr;
		while (comp)
		{
			if (comp->rank == rank)
			{
				print_export_aux(comp);
				rank++;
				break ;
			}
			comp = comp->next;
		}
		current = current->next;
	}
}
