/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:17:23 by pedperei          #+#    #+#             */
/*   Updated: 2023/04/06 17:05:32 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*envlast(t_env *env)
{
	if (!env)
		return (0);
	while (env->next)
		env = env->next;
	return (env);
}

void	envadd_back(t_env **env, t_env *new)
{
	t_env	*last;

	if (!new || !env)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	last = envlast(*env);
	last->next = new;
}

t_env	*envnew(char *env_name)
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
		env_var = envnew(env[i]);
		envadd_back(env_arr, env_var);
		i++;
	}
	return (env_arr);
}
