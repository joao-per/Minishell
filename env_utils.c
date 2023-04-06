/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:17:23 by pedperei          #+#    #+#             */
/*   Updated: 2023/04/06 17:18:35 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_last(t_env *env)
{
	if (!env)
		return (0);
	while (env->next)
		env = env->next;
	return (env);
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*last;

	if (!new || !env)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	last = env_last(*env);
	last->next = new;
}

t_env	*env_new(char *env_name)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->env_var = ft_strdup(env_name);
	new->next = NULL;
	return (new);
}

t_env	**env_init(char **env)
{
	int		i;
	t_env	**env_arr;
	t_env	*env_var;

	i = 0;
	env_arr = (t_env **)ft_calloc(1, sizeof(t_env *));
	while (env[i])
	{
		env_var = env_new(env[i]);
		env_add_back(env_arr, env_var);
		i++;
	}
	return (env_arr);
}

void	print_env_vars(t_env **env_arr)
{
	t_env	*current;

	current = *env_arr;
	while (current)
	{
		printf("%s\n", current->env_var);
		current = current->next;
	}
}
